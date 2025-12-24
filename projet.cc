/*!
  \file   projet.cc
  \author Paul Haefliger et Louis Lixi
  \date   Modification en Mai 2025
  \brief  Implémentation du module principal avec main() et GTKmm.
*/

#include <gtkmm/application.h>
#include <string>
#include "gui.h"
#include "jeu.h"

using namespace std;

int main(int argc, char *argv[])
{
    string file_name("");
    if (argc > 1)
    {
        file_name = argv[1];
    }
    auto app = Gtk::Application::create();
    return app->make_window_and_run<My_window>(1, argv, file_name);
}
