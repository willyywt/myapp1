#!/bin/python3
from jinja2 import Environment, PackageLoader, FileSystemLoader, select_autoescape
jinja_env = Environment(
        loader = FileSystemLoader(".")
        )
import os.path, sys
import re

class Project:
    def __init__ (self, projectattr):
        self.name = projectattr["name"]
        self.name_lower = projectattr["name"]
        self.name_upper = projectattr["name"].upper()
        self.header_suffix = "h"
        self.source_suffix = "c"
        self.version = projectattr["version"]
        self.prefix = projectattr["prefix"]
        self.prefix_upper = projectattr["prefix"].upper()
        self.appid = projectattr["appid"]
        self.dependency = []

    def license (self, licenseattr):
        self.license_file = licenseattr["file"];
        license_header_file = licenseattr["header"];
        with open(license_header_file, "r") as license_header_file_handle:
            self.license_header = license_header_file_handle.read()

    def dep_add (self, dep):
        self.dependency.append(dep)

pattern = re.compile(r'(?<!^)(?=[A-Z])')
def camel_to_snake(name):
    return pattern.sub('_', name).lower()

def debug_print(string):
    return

def gen_project_header_ifnull(project):
    project_header_filename = "src/" + project.name_lower + "." + project.header_suffix
    project_private_header_filename = "src/private.h"
    if os.path.exists(project_header_filename):
        debug_print(f"NOT generating project public header {project_header_filename}, but you should probably not care")
    else:
        print(f"Generating project public header file {project_header_filename}")
        template_header = jinja_env.get_template("templates/myproject.h.template")
        with open(project_header_filename, "w") as file:
            print(template_header.render(project=project), file=file)
        print(f"""This is your project's header file. First goes your project's dependency's headers, then your project classes' public headers.
If you create a new class for your project, manually add its public header to this file.
""")
    if os.path.exists(project_private_header_filename):
        debug_print(f"NOT generating project public header {project_private_header_filename}, but you should probably not care")
    else:
        print(f"Generating project public header file {project_private_header_filename}")
        template_private_header = jinja_env.get_template("templates/private.h.template")
        with open(project_private_header_filename, "w") as file:
            print(template_private_header.render(project=project), file=file)
        print(f"""This is your project's  private header file. First goes your project's public headers, then your project classes' private headers.
Add private headers here.
""")

def gen_class_ifnull(klass, project):
    # Mandatory argument:
    # @klass.name Name in camel case.
    klass_real = {}
    if not "prefix" in klass:
        klass_real["prefix"] = project.prefix
    klass_real["prefix_upper"] = klass_real["prefix"].upper()
    klass_real["camelname"] = klass_real["prefix"] + klass["name"]
    klass_real["snakename"] = camel_to_snake(klass_real["prefix"]) + "_" + camel_to_snake(klass["name"])
    klass_real["snakename_deprefix"] = camel_to_snake(klass["name"])
    klass_real["snakename_deprefix_upper"] = klass_real["snakename_deprefix"].upper()
    klass_real["file_name"] = klass_real["prefix"].lower() + klass["name"].lower()
    klass_real["header_suffix"] = project.header_suffix
    klass_real["source_suffix"] = project.source_suffix

    filename_nosuffix = "src" + "/" + klass_real["file_name"]
    filename_h = filename_nosuffix + "." + klass_real["header_suffix"]
    filename_c = filename_nosuffix + "." + klass_real["source_suffix"]
    filename_basename_h = '"' + klass_real["file_name"] + "." + klass_real["header_suffix"] + '"'
    project_filename = project.name_lower + "." + project.header_suffix
    if os.path.exists(filename_h):
        debug_print(f"NOT generating existing header {filename_h}, but you should probably not care")
    else:
        print(f"Generating header file {filename_h}")
        template_header = jinja_env.get_template("templates/gobject_header.template")
        file = open(filename_h, "w")
        print(template_header.render(gobject = klass_real, project=project), file=file)
        file.close()
        print(f"Please add {filename_basename_h} to src/{project_filename}")
    if os.path.exists(filename_c):
        debug_print(f"NOT generating existing source {filename_c}, but you should probably not care")
    else:
        print(f"Generating source file {filename_c}")
        template_source = jinja_env.get_template("templates/gobject_source.template")
        file = open(filename_c, "w")
        print(template_source.render(gobject = klass_real, project=project), file=file)
        file.close()

def gen_config_automatic(config_el, project):
    config_real = {}
    template_config = jinja_env.get_template("src/config.h.template")
    with open("src/config.h", "w") as file:
        print(template_config.render(project=project,config=config_real), file=file)

def gen_meson_automatic(proj_root, project):
    meson_proj = {}
    meson_src = {"class": [], "main": [], "compile_options": []}
    meson_data = {}
    for child in proj_root:
        # Already added to project
        if child.tag == "license":
            continue
        elif child.tag == "dependency":
            continue 
        elif child.tag == "class":
            if not "prefix" in child.attrib:
                prefixname = project.prefix
            else:
                prefixname = child.attrib["prefix"]
            filename_nosuffix = prefixname.lower() + child.attrib["name"].lower()
            meson_src["class"].append(filename_nosuffix)
        # This is already the meson target
        elif child.tag == "target":
            continue
        # currently nothing
        elif child.tag == "config_h":
            continue
        elif child.tag == "compile_option":
            meson_src["compile_options"].append(child.attrib["name"])
        elif child.tag == "gresource":
            meson_data["gresource"]=child.attrib["file"]
        elif child.tag == "gschema":
            meson_data["gschema"]=1
        elif child.tag == "main":
            filename_nosuffix = child.attrib["name"]
            meson_src["main"].append(filename_nosuffix)
        else:
            print(f"Warning: In target meson:  unrecognized tag: {child.tag}", file=sys.stderr)

    template_meson_proj = jinja_env.get_template("meson.build.template")
    with open("meson.build", "w") as file:
        print(template_meson_proj.render(project=project), file=file)
    template_meson_data = jinja_env.get_template("data/meson.build.template")
    with open("data/meson.build", "w") as file:
        print(template_meson_data.render(meson_data=meson_data), file=file)
    template_meson_src = jinja_env.get_template("src/meson.build.template")
    with open("src/meson.build", "w") as file:
        print(template_meson_src.render(meson_src=meson_src, meson_data=meson_data), file=file)

import xml.etree.ElementTree as ET
project_tree = ET.parse("project.xml")
project_root = project_tree.getroot()
project_attr = project_root.attrib

project = Project(project_attr)
for child in project_root:
    # these are project meta
    if child.tag == "license":
        project.license(child.attrib)
        gen_project_header_ifnull(project)
    elif child.tag == "dependency":
        project.dep_add(child.attrib)
    elif child.tag == "class":
        gen_class_ifnull(child.attrib, project)
    # build target
    elif child.tag == "target":
        tagname = child.attrib["name"]
        if tagname == "meson":
            gen_meson_automatic(project_root, project)
        else:
            print(f"Warning: Project unrecognized target: {tagname}", file=sys.stderr)
    elif child.tag == "config_h":
        gen_config_automatic(child, project)
    # these are handled by build targets
    elif child.tag == "main":
        continue
    elif child.tag == "compile_option":
        continue
    elif child.tag == "gresource":
        continue
    elif child.tag == "gschema":
        continue
    else:
        print(f"Warning: Project unrecognized tag: {child.tag}", file=sys.stderr)
