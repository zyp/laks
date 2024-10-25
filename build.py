# This is an erect sub-blueprint.
# Load this into your project blueprint to load the laks build system.

import sys
import pathlib
sys.path.append(str(pathlib.Path(__file__).parent / 'py_lib'))

from laks_build import *
