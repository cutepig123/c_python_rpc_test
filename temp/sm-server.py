from multiprocessing import shared_memory
import os, sys, time
import array

shm_a = shared_memory.SharedMemory(name='Global\\MyFileMappingObject')
while True:
    time.sleep(1)
    a = array.array('b', shm_a.buf[:5])
    print(a)
