

import zlib
import sys
import numpy as np

dst = np.array(np.random.randint(0, 16, int(sys.argv[2])), dtype=np.uint8)
dst.tofile(sys.argv[1])