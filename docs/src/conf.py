# pylint: disable=missing-docstring,invalid-name,redefined-builtin

import os
import subprocess

subprocess.call('cd ../../ && doxygen', shell=True)

html_theme = 'sphinx_rtd_theme'

extensions = [
    'breathe',
    'sphinx.ext.todo',
]

breathe_projects = {
    'ObjectDataBase': os.path.abspath('../xml'),
}
breathe_default_project = 'ObjectDataBase'
breathe_default_members = ('members', 'private-members', 'protected-members')

highlight_language = 'c++'

source_suffix = '.rst'
master_doc = 'index'

project = 'ObjectDataBase'
copyright = '2018, Manfred Morgner'
author = 'Manfred Morgner'

version = '0.1.17'
release = '0.1.17'

language = 'en'

exclude_patterns = [
    'build',
    'Thumbs.db',
    '.DS_Store',
]

todo_include_todos = True

htmlhelp_basename = 'ObjectDataBase'

man_pages = [
    (master_doc, 'objectdatabase', 'ObjectDataBase Documentation',
     [author], 1)
]

primary_domain = 'cpp'
