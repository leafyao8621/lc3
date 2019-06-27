#include <stdio.h>
#include <stdint.h>
#include <gtk/gtk.h>
#include "../../assembler/assembler.h"
#include "../../emulator/emulator.h"

static const gchar* fn = 0;
static GtkTextBuffer* asm_console_buf;
static GtkTextBuffer* out_console_buf;
static struct {
    GtkLabel* dec;
    GtkLabel* hex;
} reg_label[8];
static struct {
    GtkLabel* hex;
    GtkLabel* ins;
} ins_prev, ins_cur, ins_next;
static uint16_t mem_start;
static struct {
    GtkLabel* add;
    GtkLabel* val;
} mem_monitor[10];

void file_picker_handle(GtkFileChooserButton* btn, gpointer data) {
    fn = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(btn));
}

inline static void show_registers(void) {
    struct CPU cpu = get_cpu();
    char buf[20];
    for (int i = 0; i < 8; i++) {
        snprintf(buf, 20, "%hd", cpu.r[i]);
        gtk_label_set_label(reg_label[i].dec, buf);
        snprintf(buf, 20, "0x%04hX", cpu.r[i]);
        gtk_label_set_label(reg_label[i].hex, buf);
    }
}

inline static void show_ins(void) {
    struct CPU cpu = get_cpu();
    char bufi[50];
    char bufh[20];
    snprintf(bufh, 20, "0x%04hX", get_mem(cpu.pc - 1));
    gtk_label_set_label(ins_prev.hex, bufh);
    disassemble(get_mem(cpu.pc - 1), bufi);
    gtk_label_set_label(ins_prev.ins, bufi);
    snprintf(bufh, 20, "0x%04hX", get_mem(cpu.pc));
    gtk_label_set_label(ins_cur.hex, bufh);
    disassemble(get_mem(cpu.pc), bufi);
    gtk_label_set_label(ins_cur.ins, bufi);
    snprintf(bufh, 20, "0x%04hX", get_mem(cpu.pc + 1));
    gtk_label_set_label(ins_next.hex, bufh);
    disassemble(get_mem(cpu.pc + 1), bufi);
    gtk_label_set_label(ins_next.ins, bufi);
}

inline static void show_mem(void) {
    char bufa[20];
    char bufh[20];
    for (uint16_t i = 0, j = mem_start; i < 10 && j < (1 << 16); i++, j++) {
        snprintf(bufa, 20, "0x%04hX", j);
        snprintf(bufh, 20, "0x%04hX", get_mem(j));
        gtk_label_set_label(mem_monitor[i].add, bufa);
        gtk_label_set_label(mem_monitor[i].val, bufh);
    }
}

void asm_handle(GtkButton* btn, gpointer data) {
    if (!fn) return;
    GtkTextIter as, ae, os, oe;
    gtk_text_buffer_get_start_iter(asm_console_buf, &as);
    gtk_text_buffer_get_end_iter(asm_console_buf, &ae);
    gtk_text_buffer_delete(asm_console_buf, &as, &ae);
    gtk_text_buffer_get_start_iter(out_console_buf, &os);
    gtk_text_buffer_get_end_iter(out_console_buf, &oe);
    gtk_text_buffer_delete(out_console_buf, &os, &oe);
    assemble(fn, "out.obj", "out.dmp");
    gtk_text_buffer_get_end_iter(asm_console_buf, &ae);
    gtk_text_buffer_insert(asm_console_buf, &ae, err_str, -1);
    load("out.obj");
    gtk_text_buffer_get_end_iter(asm_console_buf, &ae);
    gtk_text_buffer_insert(asm_console_buf, &ae, "\n", -1);
    gtk_text_buffer_get_end_iter(asm_console_buf, &ae);
    gtk_text_buffer_insert(asm_console_buf, &ae, fn, -1);
    gtk_text_buffer_get_end_iter(asm_console_buf, &ae);
    gtk_text_buffer_insert(asm_console_buf, &ae, "\n", -1);
    gtk_text_buffer_get_end_iter(asm_console_buf, &ae);
    gtk_text_buffer_insert(asm_console_buf, &ae, "assembled and loaded", -1);
    show_registers();
    show_ins();
    mem_start = (get_cpu()).pc;
    show_mem();
}

void ld_handle(GtkButton* btn, gpointer data) {
    if (!fn) return;
    GtkTextIter ae;
    load((char*)fn);
    gtk_text_buffer_get_end_iter(asm_console_buf, &ae);
    gtk_text_buffer_insert(asm_console_buf, &ae, "\n", -1);
    gtk_text_buffer_get_end_iter(asm_console_buf, &ae);
    gtk_text_buffer_insert(asm_console_buf, &ae, fn, -1);
    gtk_text_buffer_get_end_iter(asm_console_buf, &ae);
    gtk_text_buffer_insert(asm_console_buf, &ae, "\n", -1);
    gtk_text_buffer_get_end_iter(asm_console_buf, &ae);
    gtk_text_buffer_insert(asm_console_buf, &ae, "loaded", -1);
    show_registers();
    show_ins();
    mem_start = (get_cpu()).pc;
    show_mem();
}

void step_handle(GtkButton* btn, gpointer data) {
    step(0);
    show_registers();
    show_ins();
    show_mem();
    GtkTextIter oe;
    if (get_out_flag()) {
        gtk_text_buffer_get_end_iter(out_console_buf, &oe);
        gtk_text_buffer_insert(out_console_buf, &oe, out_buf, -1);
    }
    reset_out_flag();
}

void run_handle(GtkButton* btn, gpointer data) {
    GtkTextIter oe;
    for (unhalt(); !get_halt();) {
        step(0);
        if (get_out_flag()) {
            gtk_text_buffer_get_end_iter(out_console_buf, &oe);
            gtk_text_buffer_insert(out_console_buf, &oe, out_buf, -1);
        }
        reset_out_flag();
    }
    show_registers();
    show_ins();
    show_mem();
}

void mem_mov_handle(GtkButton* btn, gpointer data) {
    _Bool is_up = *(_Bool*)data;
    if (is_up && mem_start) {
        mem_start -= 10;
    } else if (mem_start < (1 << 16)) {
        mem_start += 10;
    }
    show_mem();
}
int main(int argc, char** argv) {
    GtkBuilder* builder;
    GError* error = NULL;
    GObject* window;
    
    gtk_init(&argc, &argv);
    builder = gtk_builder_new();
    if (!gtk_builder_add_from_resource(builder,
                                       "/com/example/lc3sim-gtk/main.ui",
                                       &error)) {
        g_printerr ("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }
    window = gtk_builder_get_object(builder, "window");
    g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
    asm_console_buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(
            gtk_builder_get_object(builder, "asm_console")
        ));
    out_console_buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(
            gtk_builder_get_object(builder, "out_console")
        ));
    char bufd[50], bufh[50];
    for (int i = 0; i < 8; i++) {
        snprintf(bufd, 50, "r%d_value_d", i);
        snprintf(bufh, 50, "r%d_value_h", i);
        reg_label[i].dec = GTK_LABEL(gtk_builder_get_object(builder, bufd));
        reg_label[i].hex = GTK_LABEL(gtk_builder_get_object(builder, bufh));
    }
    ins_prev.hex = GTK_LABEL(gtk_builder_get_object(builder, "ins_prev_h"));
    ins_prev.ins = GTK_LABEL(gtk_builder_get_object(builder, "ins_prev_i"));
    ins_cur.hex = GTK_LABEL(gtk_builder_get_object(builder, "ins_cur_h"));
    ins_cur.ins = GTK_LABEL(gtk_builder_get_object(builder, "ins_cur_i"));
    ins_next.hex = GTK_LABEL(gtk_builder_get_object(builder, "ins_next_h"));
    ins_next.ins = GTK_LABEL(gtk_builder_get_object(builder, "ins_next_i"));
    char bufa[50], bufv[50];
    for (int i = 0; i < 10; i++) {
        snprintf(bufa, 50, "mem_add_%d", i);
        snprintf(bufv, 50, "mem_val_%d", i);
        mem_monitor[i].add = GTK_LABEL(gtk_builder_get_object(builder, bufa));
        mem_monitor[i].val = GTK_LABEL(gtk_builder_get_object(builder, bufv));
    }
    g_signal_connect(gtk_builder_get_object(builder, "file_picker"), "file-set",
                     G_CALLBACK(file_picker_handle), 0);
    g_signal_connect(gtk_builder_get_object(builder, "asm_btn"), "clicked",
                     G_CALLBACK(asm_handle), 0);
    g_signal_connect(gtk_builder_get_object(builder, "ld_btn"), "clicked",
                     G_CALLBACK(ld_handle), 0);
    g_signal_connect(gtk_builder_get_object(builder, "step_btn"), "clicked",
                     G_CALLBACK(step_handle), 0);
    g_signal_connect(gtk_builder_get_object(builder, "run_btn"), "clicked",
                     G_CALLBACK(run_handle), 0);
    _Bool t, f;
    t = 1;
    f = 0;
    g_signal_connect(gtk_builder_get_object(builder, "mem_add_up"), "clicked",
                     G_CALLBACK(mem_mov_handle), &t);
    g_signal_connect(gtk_builder_get_object(builder, "mem_add_down"), "clicked",
                     G_CALLBACK(mem_mov_handle), &f);
    gtk_main();
    
    return 0;
}