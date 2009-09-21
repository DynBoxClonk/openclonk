/* mape - C4 Landscape.txt editor
 * Copyright (C) 2005 Armin Burgmeier
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdlib.h>
#include <signal.h>
#include <gtk/gtkmessagedialog.h>
#include <glib/gthread.h>
#include <gdk/gdk.h>
#include "editview.h"
#include "window.h"

static MapeWindow** _global_wnd = NULL;

static const gchar* SEGV_MSG =
	"Mape saved the currently opened file to %s\n\n"
	"If the crash is reproducible, I would like to fix it. Drop an email "
	"with steps to reproduce and a possible file that caused the crash to "
	"Armin Burgmeier <armin@arbur.net>";

static const gchar* SEGV_MSG_ERR =
	"Mape failed to save the currently opened file to %s: %s\n\n"
	"If the crash is reproducible, I would like to fix it. Drop an email "
	"with steps to reproduce and a possible file that caused the crash to "
	"Armin Burgmeier <armin@arbur.net>";

static void segv_handler()
{
	GtkWidget* error_dialog;
	gboolean result;
	MapeWindow* wnd;

	gchar* filename;
	gchar* utf8_file;
	gchar* current;

	GError* error;
	error = NULL;

	if(_global_wnd == NULL)
		return;

	wnd = *_global_wnd;
	if(wnd->edit_view->file_path)
	{
		filename = g_strdup(wnd->edit_view->file_path);	
	}
	else
	{
		current = g_get_current_dir();
		filename = g_build_filename(
			current,
			"Landscape_crash.txt",
			NULL
		);

		g_free(current);
	}

	result = mape_edit_view_save(wnd->edit_view, filename, &error);

	utf8_file = g_filename_to_utf8(
		filename,
		-1,
		NULL,
		NULL,
		NULL
	);

	g_free(filename);

	error_dialog = gtk_message_dialog_new(
		NULL,
		GTK_DIALOG_MODAL,
		GTK_MESSAGE_ERROR,
		GTK_BUTTONS_OK,
		"The application crashed!"
	);

	if(result == TRUE)
	{
		gtk_message_dialog_format_secondary_text(
			GTK_MESSAGE_DIALOG(error_dialog),
			SEGV_MSG,
			utf8_file
		);
	}
	else
	{	
		gtk_message_dialog_format_secondary_text(
			GTK_MESSAGE_DIALOG(error_dialog),
			SEGV_MSG_ERR,
			utf8_file,
			error->message
		);

		g_error_free(error);
	}

	g_free(utf8_file);

	gtk_dialog_run(GTK_DIALOG(error_dialog) );
	gtk_widget_destroy(error_dialog);

	exit(EXIT_FAILURE);
}

static void mape_main_window_destroy(GtkWidget* widget,
                                     gpointer data)
{
	gtk_main_quit();
}

int main(int argc,
         char* argv[])
{
	MapeWindow* window;
	GError* error = NULL;
	GtkWidget* error_dialog;

	_global_wnd = &window;

	signal(SIGSEGV, segv_handler);
	
	g_thread_init(NULL);
	gdk_threads_init();
	gdk_threads_enter();

	gtk_init(&argc, &argv);

	/* TODO: Set global application icon */
	window = mape_window_new(argc, argv, &error);

	if(error != NULL)
	{
		error_dialog = gtk_message_dialog_new(
			NULL,
			GTK_DIALOG_MODAL,
			GTK_MESSAGE_ERROR,
			GTK_BUTTONS_OK,
			"Application initialization failed"
		);

		gtk_message_dialog_format_secondary_text(
			GTK_MESSAGE_DIALOG(error_dialog),
			"%s",
			error->message
		);

		gtk_window_set_title(GTK_WINDOW(error_dialog), "Mape");

		g_error_free(error);
		gtk_dialog_run(GTK_DIALOG(error_dialog) );
		gtk_widget_destroy(error_dialog);

		return EXIT_FAILURE;
	}

	g_signal_connect(
		G_OBJECT(window->window),
		"destroy",
		G_CALLBACK(mape_main_window_destroy),
		(gpointer)window
	);

	gtk_main();
	mape_window_destroy(window);
	gdk_threads_leave();

	return EXIT_SUCCESS;
}
