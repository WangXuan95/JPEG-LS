# -*- coding: utf-8 -*-
# Python3

# python standard libraries importation
import sys
import os
import subprocess as sp

# python third-party libraries importation (additional installation is required, if you do not have)
import numpy as np
import pillow_jpls      # run "python -m pip install pillow-jpls"      to install
from PIL import Image   # run "python -m pip install Pillow"           to install




TMP_PGM_FILE = 'tmp.pgm'
TMP_JLS_FILE = 'tmp.jls'


#JLS_BIN = './JLSencode'                # for linux
JLS_BIN = '.\\JLSencode.exe'          # for windows


def callJLSencoder (pgm_file_name, jls_file_name, near) :
    COMMANDS = [ JLS_BIN, pgm_file_name, jls_file_name, str(near) ]
    p = sp.Popen(COMMANDS, stdin=sp.PIPE, stdout=sp.PIPE, stderr=sp.PIPE)     # call the HEVCencoder executable file
    assert p.wait() == 0



def convertImageToPGM (image_file_name, pgm_file_name) :
    img_obj = Image.open(image_file_name)
    img_mono = img_obj.convert('L')
    img_obj.close()
    img_mono.save(pgm_file_name)



def compare2Images(image1_file_name, image2_file_name) :
    img_obj = Image.open(image1_file_name)
    img_mono = img_obj.convert('L')
    img_obj.close()
    img1 = np.asarray(img_mono)
    
    img_obj = Image.open(image2_file_name)
    img_mono = img_obj.convert('L')
    img_obj.close()
    img2 = np.asarray(img_mono)
    
    h1, w1 = img1.shape
    h2, w2 = img2.shape
    
    assert h1 == h2, 'different height: %d %d' % (h1, h2)
    assert w1 == w2, 'different width : %d %d' % (w1, w2)
    
    return np.max(np.abs(np.int32(img1) - np.int32(img2)))




if __name__ == '__main__' :
    
    try :
        in_dir = sys.argv[1]
    except :
        print('Usage: python %s <input_dir_name>' % sys.argv[0])
        exit(-1)
    
    for fname in os.listdir(in_dir) :
        fname_full = in_dir + os.path.sep + fname
        
        try :
            convertImageToPGM(fname_full, TMP_PGM_FILE)
        except :
            print('skip %s' % fname)
            continue
        
        for near in range(0, 10) :
            try :
                callJLSencoder(TMP_PGM_FILE, TMP_JLS_FILE, near)
            except AssertionError as e:
                print('*** %s JLS compression error' % fname)
                continue
            
            try :
                max_diff = compare2Images(TMP_PGM_FILE, TMP_JLS_FILE)
            except AssertionError as e:
                print('*** %s verify failed: %s' % (fname, str(e)) )
                continue
            
            if max_diff > near :
                print('*** %s verify failed: near = %d, max_diff = %d' % (fname, near, max_diff) )
            else :
                print(    '%s verify passed: near = %d, max_diff = %d' % (fname, near, max_diff) )
        
        os.remove(TMP_PGM_FILE)
        os.remove(TMP_JLS_FILE)
