#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    GtkWidget *window;
    GtkWidget *fixed_radio;
    GtkWidget *variable_radio;
    GtkWidget *size_entry;
    GtkWidget *min_entry;
    GtkWidget *max_entry;
    GtkWidget *input_text;
    GtkWidget *output_text;
    GtkWidget *split_button;
} AppWidgets;

static void split_string(GtkWidget *widget, gpointer data) {
    AppWidgets *widgets = (AppWidgets *)data;
    
    // Get input text
    GtkTextBuffer *input_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widgets->input_text));
    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(input_buffer, &start);
    gtk_text_buffer_get_end_iter(input_buffer, &end);
    gchar *input_str = gtk_text_buffer_get_text(input_buffer, &start, &end, FALSE);
    
    if (input_str == NULL || strlen(input_str) == 0) {
        g_free(input_str);
        return;
    }
    
    // Clear output
    GtkTextBuffer *output_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widgets->output_text));
    gtk_text_buffer_set_text(output_buffer, "", -1);
    
    int len = strlen(input_str);
    GString *output = g_string_new("");
    
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->fixed_radio))) {
        // Fixed length splitting
        const gchar *size_text = gtk_entry_get_text(GTK_ENTRY(widgets->size_entry));
        int n = atoi(size_text);
        
        if (n <= 0) {
            g_string_append(output, "Error: Size must be greater than 0\n");
        } else {
            for (int i = 0; i < len; i += n) {
                g_string_append(output, "Str = Str + \"");
                for (int j = i; j < i + n && j < len; j++) {
                    g_string_append_c(output, input_str[j]);
                }
                g_string_append(output, "\"\n");
            }
        }
    } else {
        // Variable size splitting
        const gchar *min_text = gtk_entry_get_text(GTK_ENTRY(widgets->min_entry));
        const gchar *max_text = gtk_entry_get_text(GTK_ENTRY(widgets->max_entry));
        int min_size = atoi(min_text);
        int max_size = atoi(max_text);
        
        if (min_size <= 0 || max_size <= 0 || min_size > max_size) {
            g_string_append(output, "Error: Invalid min/max bounds\n");
        } else {
            srand(time(NULL));
            int i = 0;
            while (i < len) {
                int chunk_size = min_size + (rand() % (max_size - min_size + 1));
                if (i + chunk_size > len) {
                    chunk_size = len - i;
                }
                
                g_string_append(output, "Str = Str + \"");
                for (int j = i; j < i + chunk_size && j < len; j++) {
                    g_string_append_c(output, input_str[j]);
                }
                g_string_append(output, "\"\n");
                
                i += chunk_size;
            }
        }
    }
    
    // Set output text
    gtk_text_buffer_set_text(output_buffer, output->str, -1);
    
    g_string_free(output, TRUE);
    g_free(input_str);
}

static void on_mode_changed(GtkWidget *widget, gpointer data) {
    AppWidgets *widgets = (AppWidgets *)data;
    gboolean is_fixed = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->fixed_radio));
    
    gtk_widget_set_sensitive(widgets->size_entry, is_fixed);
    gtk_widget_set_sensitive(widgets->min_entry, !is_fixed);
    gtk_widget_set_sensitive(widgets->max_entry, !is_fixed);
}

static void on_destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    
    AppWidgets widgets;
    
    // Create main window
    widgets.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(widgets.window), "String Splitter 9000");
    gtk_window_set_default_size(GTK_WINDOW(widgets.window), 800, 600);
    g_signal_connect(widgets.window, "destroy", G_CALLBACK(on_destroy), NULL);
    
    // Create main container
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(widgets.window), vbox);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);
    
    // Mode selection
    GtkWidget *mode_frame = gtk_frame_new("Split Mode");
    gtk_box_pack_start(GTK_BOX(vbox), mode_frame, FALSE, FALSE, 0);
    
    GtkWidget *mode_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_container_add(GTK_CONTAINER(mode_frame), mode_box);
    gtk_container_set_border_width(GTK_CONTAINER(mode_box), 10);
    
    widgets.fixed_radio = gtk_radio_button_new_with_label(NULL, "Fixed Length");
    widgets.variable_radio = gtk_radio_button_new_with_label_from_widget(
        GTK_RADIO_BUTTON(widgets.fixed_radio), "Variable Size");
    
    gtk_box_pack_start(GTK_BOX(mode_box), widgets.fixed_radio, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(mode_box), widgets.variable_radio, FALSE, FALSE, 0);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets.fixed_radio), TRUE);
    
    g_signal_connect(widgets.fixed_radio, "toggled", G_CALLBACK(on_mode_changed), &widgets);
    g_signal_connect(widgets.variable_radio, "toggled", G_CALLBACK(on_mode_changed), &widgets);
    
    // Options frame
    GtkWidget *options_frame = gtk_frame_new("Options");
    gtk_box_pack_start(GTK_BOX(vbox), options_frame, FALSE, FALSE, 0);
    
    GtkWidget *options_grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(options_frame), options_grid);
    gtk_container_set_border_width(GTK_CONTAINER(options_grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(options_grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(options_grid), 10);
    
    // Fixed size option
    GtkWidget *size_label = gtk_label_new("Size:");
    widgets.size_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(widgets.size_entry), "50");
    gtk_grid_attach(GTK_GRID(options_grid), size_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(options_grid), widgets.size_entry, 1, 0, 1, 1);
    
    // Variable size options
    GtkWidget *min_label = gtk_label_new("Min:");
    widgets.min_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(widgets.min_entry), "30");
    gtk_widget_set_sensitive(widgets.min_entry, FALSE);
    
    GtkWidget *max_label = gtk_label_new("Max:");
    widgets.max_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(widgets.max_entry), "70");
    gtk_widget_set_sensitive(widgets.max_entry, FALSE);
    
    gtk_grid_attach(GTK_GRID(options_grid), min_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(options_grid), widgets.min_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(options_grid), max_label, 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(options_grid), widgets.max_entry, 3, 1, 1, 1);
    
    // Input frame
    GtkWidget *input_frame = gtk_frame_new("Enter the string to be split:");
    gtk_box_pack_start(GTK_BOX(vbox), input_frame, TRUE, TRUE, 0);
    
    GtkWidget *input_scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(input_frame), input_scrolled);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(input_scrolled),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(input_scrolled), 150);
    
    widgets.input_text = gtk_text_view_new();
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(widgets.input_text), GTK_WRAP_WORD);
    gtk_container_add(GTK_CONTAINER(input_scrolled), widgets.input_text);
    
    // Split button
    widgets.split_button = gtk_button_new_with_label("Split String");
    g_signal_connect(widgets.split_button, "clicked", G_CALLBACK(split_string), &widgets);
    gtk_box_pack_start(GTK_BOX(vbox), widgets.split_button, FALSE, FALSE, 0);
    
    // Output frame
    GtkWidget *output_frame = gtk_frame_new("Output:");
    gtk_box_pack_start(GTK_BOX(vbox), output_frame, TRUE, TRUE, 0);
    
    GtkWidget *output_scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(output_frame), output_scrolled);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(output_scrolled),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(output_scrolled), 200);
    
    widgets.output_text = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(widgets.output_text), FALSE);
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(widgets.output_text), TRUE);
    gtk_container_add(GTK_CONTAINER(output_scrolled), widgets.output_text);
    
    // Show all widgets
    gtk_widget_show_all(widgets.window);
    
    // Initialize mode
    on_mode_changed(widgets.fixed_radio, &widgets);
    
    gtk_main();
    
    return 0;
}

