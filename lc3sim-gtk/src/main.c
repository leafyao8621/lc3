#include <gtk/gtk.h>
#include "../../assembler/assembler.h"
#include "../../emulator/emulator.h"

void asm_handle(GtkButton* btn, gpointer data) {
    puts(data);
}

int main(int argc, char** argv) {
    GtkBuilder* builder;
    GError* error = NULL;
    GObject* window;
    GtkTextBuffer* asm_console_buf;
    GtkTextBuffer* out_console_buf;
    gtk_init(&argc, &argv);
    builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, "./ui/main.ui", &error)) {
        g_printerr ("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }
    window = gtk_builder_get_object (builder, "window");
    g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
    asm_console_buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(
            gtk_builder_get_object(builder, "asm_console")
        ));
    out_console_buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(
            gtk_builder_get_object(builder, "out_console")
        ));
    // assemble("../test/fibb.asm", "fibb.obj", "fibb.dmp");
    // gtk_text_buffer_insert_at_cursor(asm_console_buf, err_str, -1);
    // load("fibb.obj");
    // for (unhalt(); !get_halt();) {
    //     step();
    //     if (get_out_flag()) {
    //         gtk_text_buffer_insert_at_cursor(out_console_buf, out_buf, -1);
    //     }
    //     reset_out_flag();
    // }
    // run(0);
    g_signal_connect_data(gtk_builder_get_object(builder, "asm_btn"), "clicked",
                          G_CALLBACK(asm_handle),
                          (gpointer)
                          gtk_file_chooser_get_filename(
                              GTK_FILE_CHOOSER(
                                  gtk_builder_get_object(builder,
                                                         "file_picker")
                              )
                          ),
                          0, 0
                    );
    gtk_main();
    
    return 0;
}