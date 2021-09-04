# My Project 
This is a project skeleton example. C source comes from [GTK Example](https://gitlab.gnome.org/GNOME/gtk/-/tree/4.2.1/examples/application9), with modifications to become a skeleton.

## Template
Use the [jinja template engine](https://jinja.palletsprojects.com/) to generate files. This is a powerful tool, but powerful tools can also kill us, so we must do this carefully.

Project information is stored in "project.xml" file. The example project aims to use an declarative xml file to declare project information, and generate build system files from the project xml file. It is mainly informational rather than formal: even if a tag in project xml file is not currently exported to build system files, it can serve as hint that "this project use xxx feature", and support can be added later. Formalization of C source/header files is not a goal: only use templates to generate initial example, and from later on these are edited by hand.

The "classgen.py" generates:
- meson build files
- C source/header skeleton, if not present
