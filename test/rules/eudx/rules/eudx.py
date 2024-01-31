"""
EU-DX contest
https://www.eudx-contest.com/
"""
import re

EU_REGION_PATTERN = re.compile('[A-Z]{2}\d{2}')  # two letters and two numbers

def check_exchange(qso):
    xchg = qso.exchange.strip()
    dxcc = tlf.get_dxcc(qso.call)
    mult = dxcc.main_prefix

    if EU_REGION_PATTERN.match(xchg):
        mult += ' ' + xchg

    return {'mult1_value': mult}
