#include <gtk/gtk.h>
#include "../../assembler/assembler.h"
#include "../../emulator/emulator.h"

int main(int argc, char** argv) {
    GtkBuilder* builder;
    GError* error = NULL;
    gtk_init(&argc, &argv);
    builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, "main.ui", &error)) {
        g_printerr ("Error loading file: %s\n", error->message);
        g_clear_error (&error);
        return 1;
    }
    gtk_main();
    
    return 0;
}