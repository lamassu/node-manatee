'use strict';

var manatee = require('./build/Release/manatee');
var MWB_CODE_MASK_PDF = 0x00000040;
var MWB_CODE_MASK_QR  = 0x00000001;

export.version = manatee.version;
export.register = manatee.register;
export.scan = manatee.scan;
