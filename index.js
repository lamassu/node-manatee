'use strict';

const manatee = require('./build/Release/manatee.node');

const CODENAMES = {
  qr: 0x00000001,
  pdf417: 0x00000040
};

exports.scanningLevel = 3;

exports.version = () => {
  const v = manatee.version();
  const M = (v >> 16) & 0xff
  const m = (v >> 8) & 0xff
  const p = (v >> 0) & 0xff
  return `${M}.${m}.${p}`
}

exports.register = function register(codeName, username, key) {
  const codeMask = CODENAMES[codeName];
  if (!codeMask) throw new TypeError('Unrecognized codeName: ' + codeName);

  const result = manatee.register(codeMask, username, key);
  if (result === 0) return;

  switch (result) {
    case -3: throw new Error('Decoder type or registration not supported');
    default: throw new Error('Registration failed');
  }
};

exports.scan = function scan(image, width, height, codeName) {
  if (typeof(width) !== 'number') throw new TypeError("Image width must be a number")
  if (typeof(height) !== 'number') throw new TypeError("Image height must be a number")

  const codeMask = CODENAMES[codeName];
  if (!codeMask) throw new TypeError('Unrecognized codeName: ' + codeName);
  return manatee.scan(image, width, height, codeMask, exports.scanningLevel);
};

exports.scanQR = function scanQR(image, width, height) {
  return exports.scan(image, width, height, 'qr');
};

exports.scanPDF417 = function scanPDF417(image, width, height) {
  return exports.scan(image, width, height, 'pdf417');
};
