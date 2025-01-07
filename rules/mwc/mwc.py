"""
OK1WC Memorial (MWC)
https://memorial-ok1wc.cz/index.php?page=rules2l
"""

import re
import string

# match trailing call modifier (e.g. /8, /P, /MM, /QRP, etc.)
MODIFIER_PATTERN = re.compile('/(\d|[A-Z]+)$')

def check_exchange(qso):
    call = MODIFIER_PATTERN.sub('', qso.call)   # remove modifier
    if len(call) > 1:
        mult = call[-1]     # last character of the call
    else:
        mult = ''

    return {'mult1_value': mult}

def get_multiplier_info():
    # A..Z and 0..9 in groups of five
    header = re.sub(r'(\w{5})', r'\1 ', string.ascii_uppercase)
    header += '  '
    header += re.sub(r'(\w{5})', r'\1 ', string.digits)
    # A --> {A}
    values = re.sub(r'(\w)', r'{\1}', header)
    mults = ['     ' + header,
        '40m:{@40} ' + values,
        '80m:{@80} ' + values,
    ]
    return mults

