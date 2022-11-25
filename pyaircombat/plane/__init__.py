import os

from pyaircombat.plane.jsb_plane import Plane
from pyaircombat.plane.jsbsim_catalog import JsbsimCatalog
from pyaircombat.plane.property import Property

if "JSBSIM_ROOT_DIR" not in os.environ:
    os.environ["JSBSIM_ROOT_DIR"] = os.path.join(os.path.dirname(__file__), "jsbsim-1.1.12")
    os.environ['JSBSIM_DEBUG'] = '0'
