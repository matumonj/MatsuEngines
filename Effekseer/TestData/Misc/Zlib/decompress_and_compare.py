import zlib
import sys
import numpy as np

compressed = np.fromfile(sys.argv[1], dtype='uint8')
original = np.fromfile(sys.argv[2], dtype='uint8')

decompressed_data = zlib.decompress(compressed)

decompressed = np.frombuffer(decompressed_data, dtype=np.uint8)

print(original.shape)
print(decompressed.shape)

print(np.array_equal(original, decompressed))