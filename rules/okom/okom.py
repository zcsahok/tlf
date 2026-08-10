"""
OK-OM DX Contest
http://okomdx.crk.cz/index.php?page=CW-rules-english
"""

OKOM_DISTRICTS = {
    'OK': [
        # Praha
        'APA','APB','APC','APD','APE','APF','APG','APH','API','APJ',
        # Central Bohemia
        'BBN','BBE','BKD','BKO','BKH','BME','BMB','BNY','BPZ','BPV','BPB','BRA',
        # Southern Bohemia
        'CBU','CCK','CJH','CPE','CPI','CPR','CST','CTA',
        # Western Bohemia
        'DDO','DCH','DKV','DKL','DPM','DPJ','DPS','DRO','DSO','DTA',
        # Northern Bohemia
        'ECL','EDE','ECH','EJA','ELI','ELT','ELO','EMO','ETE','EUL',
        # Eastern Bohemia
        'FHB','FHK','FCR','FJI','FNA','FPA','FRK','FSE','FSV','FTR','FUO',
        # Southern Moravia
        'GBL','GBM','GBV','GBR','GHO','GJI','GKR','GPR','GTR','GUH','GVY','GZL','GZN', 'GZS',
        # Northern Moravia
        'HBR','HFM','HJE','HKA','HNJ','HOL','HOP','HOS','HPR','HSU','HVS'
    ],
    'OM': [
        # Bratislava, prefix OM1
        'BAA','BAB','BAC','BAD','BAE','MAL','PEZ','SEN',
        # Trnava, prefix OM2
        'TRN','DST','GAL','HLO','PIE','SEA','SKA',
        # Trencin, prefix OM4
        'TNC','BAN','ILA','MYJ','NMV','PAR','PBY','PRI','PUC',
        # Nitra, prefix OM5
        'NIT','KOM','LVC','NZA','SAL','TOP','ZMO',
        # Zilina, prefix OM6
        'ZIL','BYT','CAD','DKU','KNM','LMI','MAR','NAM','RUZ','TTE','TVR',
        # Banska Bystrica, prefix OM7
        'BBY','BRE','DET','KRU','LUC','POL','REV','RSO','VKR','ZVO','ZAR','ZIH','BST',
        # Kosice, prefix OM8
        'KEA','KEB','KEC','KED','KEO','GEL','MIC','ROZ','SOB','SNV','TRE',
        # Presov, prefix OM0
        'PRE','BAR','HUM','KEZ','LEV','MED','POP','SAB','SNI','SLU','STR','SVI','VRT'
    ]
}

MY_PREFIX = None
MY_CONTINENT = None

def init(cfg):
    dxcc = tlf.get_dxcc(tlf.MY_CALL)
    global MY_PREFIX
    MY_PREFIX = dxcc.main_prefix
    global MY_CONTINENT
    MY_CONTINENT = dxcc.continent


def score(qso):
    # 6.3 /MM stations are not multipliers,
    # but worth 5 points for any participant.
    if (qso.call.endswith('/MM')):
        return 5

    dxcc = tlf.get_dxcc(qso.call)

    if MY_PREFIX in ['OK','OM']:
        # 6.1 Czech and Slovak station:
        # - QSO with your own country - 2 points,
        # - QSO with a different country on your continent - 3 points,
        # - QSO with another continent - 5 points.
        if dxcc.main_prefix == MY_PREFIX:
            points = 2
        elif dxcc.continent == MY_CONTINENT:
            points = 3
        else:
            points = 5
    else:
        # 6.2 Non-Czech and Slovak stations:
        # - QSO with Czech and Slovak station - 10 points,
        # - QSO with your own country - 1 points,
        # - QSO with a different country on your continent - 3 points,
        # - QSO with another continent - 5 points.
        if dxcc.main_prefix in ['OK','OM']:
            points = 10
        elif dxcc.main_prefix == MY_PREFIX:
            points = 1
        elif dxcc.continent == MY_CONTINENT:
            points = 3
        else:
            points = 5

    return points


def check_exchange(qso):
    xchg = qso.exchange.strip()
    dxcc = tlf.get_dxcc(qso.call)

    mult = dxcc.main_prefix

    if dxcc.main_prefix in ['OK','OM']:
        if xchg in OKOM_DISTRICTS[dxcc.main_prefix]:
            mult += ' ' + xchg
    else:
        xchg = xchg.rjust(3, '0')   # normalize serial number

    return {'mult1_value': mult, 'normalized_exchange': xchg}

