// Author: Yu Chen <yu.chen@live.ie>
// License: Apache License 2.0

'use strict';

module.exports = {
  readIdCard: function (uuid, onSuccess, onError) {
    cordova.exec(onSuccess, onError, "Kaer", "readIdCard", [uuid]);
  }
};