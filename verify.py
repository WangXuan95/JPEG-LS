# -*- coding: utf-8 -*-
# Python3

# python standard libraries importation
import sys
import os

# python third-party libraries importation (additional installation is required, if you do not have)
import numpy as np
from PIL import Image   # run "python -m pip install Pillow"      to install
import pillow_jpls      # run "python -m pip install pillow-jpls" to install


TMP_PNM_FILE = 'tmp.pnm'
TMP_JLS_FILE = 'tmp.jls'


#JLS_BIN = 'wsl ./JLSencode'           # for linux (use windows subsystem of linux (WSL))
JLS_BIN = '.\\JLSencode.exe'          # for windows


def callJLSencoder (pnm_file_name, jls_file_name, near) :
    command = '%s %s %s %d' % (JLS_BIN, pnm_file_name, jls_file_name, near)
    assert os.system(command) == 0


def convertImageToPNM (image_file_name, pnm_file_name) :
    img_obj = Image.open(image_file_name)
    img_obj.save(pnm_file_name)
    img_obj.close()


def compare2Images(image1_file_name, image2_file_name) :
    img_obj = Image.open(image1_file_name)
    h1, w1 = img_obj.height, img_obj.width
    img1 = np.asarray(img_obj)
    img_obj.close()
    
    img_obj = Image.open(image2_file_name)
    h2, w2 = img_obj.height, img_obj.width
    img2 = np.asarray(img_obj)
    img_obj.close()
    
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
            convertImageToPNM(fname_full, TMP_PNM_FILE)
        except :
            print('skip %s' % fname)
            continue
        
        for near in range(0, 6) :
            try :
                callJLSencoder(TMP_PNM_FILE, TMP_JLS_FILE, near)
            except AssertionError as e:
                print('*** %s JLS compression error' % fname)
                continue
            
            try :
                max_diff = compare2Images(TMP_PNM_FILE, TMP_JLS_FILE)
            except AssertionError as e:
                print('*** %s verify failed: %s' % (fname, str(e)) )
                continue
            
            if max_diff > near :
                print('*** %s verify failed: near = %d, max_diff = %d' % (fname, near, max_diff) )
            else :
                print(    '%s verify passed: near = %d, max_diff = %d' % (fname, near, max_diff) )
        
        os.remove(TMP_PNM_FILE)
        os.remove(TMP_JLS_FILE)
