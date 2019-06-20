#include <gtk/gtk.h>
#include "../../assembler/assembler.h"
#include "../../emulator/emulator.h"

const gchar* fn = 0;
GtkTextBuffer* asm_console_buf;
GtkTextBuffer* out_console_buf;

void file_picker_handle(GtkFileChooserButton* btn, gpointer data) {
    fn = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(btn));
}

void asm_handle(GtkButton* btn, gpointer data) {
    if (!fn) return;
    assemble(fn, "out.obj", "out.dmp");
    gtk_text_buffer_insert_at_cursor(asm_console_buf, err_str, -1);
    load("out.obj");
    for (unhalt(); !get_halt();) {
        step();
        if (get_out_flag()) {
            printf("%s", out_buf);
            gtk_text_buffer_insert_at_cursor(out_console_buf, out_buf, -1);
        }
        reset_out_flag();
    }
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
    
    g_signal_connect(gtk_builder_get_object(builder, "file_picker"), "file-set",
                     G_CALLBACK(file_picker_handle), 0);
    g_signal_connect(gtk_builder_get_object(builder, "asm_btn"), "clicked",
                     G_CALLBACK(asm_handle), 0);
    gtk_main();
    
    return 0;
}