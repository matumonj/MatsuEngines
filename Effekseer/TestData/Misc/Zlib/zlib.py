import zlib
import sys
import numpy as np

buf = np.fromfile(sys.argv[1], dtype='uint8')

compress_data = zlib.compress(buf, level=6)

dst = np.array(compress_data)

dst.tofile(sys.argv[2])