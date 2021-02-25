const BUTTON_PRESSED = 127;
const BUTTON_RELEASED = 0;


function DBG(str) {
  print("[MixxxFighter] " + str);
}


function groupFromChannelIndex(channelIndex) {
  return '[Channel' + String(channelIndex) + ']';
}


var MixxxFighter = {};
MixxxFighter.selectedChannel = [false, false, false, false];
MixxxFighter.activeChannel = -1;
MixxxFighter.modifierPage = -1;
MixxxFighter.shift = false;
MixxxFighter.buttonPressDuringChannelSelect = 0;

MixxxFighter.init = function(id, debugging) {
  DBG("Hello from MixxxFighter!");
  switchChannel(0);
  switchModifierPage(0);
  for (var i = 1; i < 5; i++) {
    var group = groupFromChannelIndex(i);
    engine.setValue(group, 'quantize', 1);
    engine.setValue(group, 'sync_enabled', 1);
  }
}

function selectChannel(channelIndex) {
  DBG("Select channel " + channelIndex);
  midi.sendShortMsg(0x90, channelIndex, 0x1);
  MixxxFighter.selectedChannel[channelIndex] = true;
  DBG(MixxxFighter.selectedChannel);
}

function unselectChannel(channelIndex) {
  DBG("Unselect channel " + channelIndex);
  midi.sendShortMsg(0x80, channelIndex, 0x1);
  MixxxFighter.selectedChannel[channelIndex] = false;
  DBG(MixxxFighter.selectedChannel);
}

function switchChannel(channelIndex) {
  DBG("Switch channel from " + MixxxFighter.activeChannel + " to " + channelIndex);
  midi.sendShortMsg(0x80, MixxxFighter.activeChannel, 0x1);
  MixxxFighter.activeChannel = channelIndex;
  midi.sendShortMsg(0x90, MixxxFighter.activeChannel, 0x1);
}

function countSelectedChannel() {
  var i = 0;
  forEachSelectedChannel(function() { i++ });
  return i;
}

function hasSelectedNonActiveChannel() {
  for (var i = 0; i < MixxxFighter.selectedChannel.length; i++) {
    if (MixxxFighter.selectedChannel[i] === true) return true;
  }
  return false;
}


function forEachSelectedChannel(cb) {
  var _hasSelectedNonActiveChannel = hasSelectedNonActiveChannel();
  if(_hasSelectedNonActiveChannel) {
    for (var i = 0; i < MixxxFighter.selectedChannel.length; i++) {
      MixxxFighter.selectedChannel[i] == true && cb(i, groupFromChannelIndex(i + 1));
    }
  } else {
    cb(i, groupFromChannelIndex(MixxxFighter.activeChannel + 1));
  }
}

function engineSetValueForSelectedChannels(key, value) {
  DBG("engineSetValueForSelectedChannels Hello" + key + " " + value);
  forEachSelectedChannel(function (i, group) {
    DBG("engineSetValueForSelectedChannels " + String(i) + " " + key + " " + value);
    engine.setValue(group, key, value)
  });
}

function registerButtonPressDuringChannelSelect() {
  if (hasSelectedNonActiveChannel()) {
    MixxxFighter.buttonPressDuringChannelSelect = true;
  }
}

function unregisterButtonPressDuringChannelSelect() {
  if (!hasSelectedNonActiveChannel()) {
    MixxxFighter.buttonPressDuringChannelSelect = false;
  }
}

function Button(cb) {
  return function (channel, control, value, status, group) {
    registerButtonPressDuringChannelSelect();
    cb(channel, control, value, status, group);
  }
}

function channelButton(buttonChannelIndex, value) {
  return function (channel, control, value, status, group) {
    if (value == BUTTON_PRESSED) {
      selectChannel(buttonChannelIndex);
    } else if (value == BUTTON_RELEASED) {
      unselectChannel(buttonChannelIndex);
      if (MixxxFighter.shift) {
        MixxxFighter.selectedChannel[buttonChannelIndex] = false
        engine.setValue(groupFromChannelIndex(buttonChannelIndex + 1), 'CloneFromDeck', MixxxFighter.activeChannel + 1);
      } else if (MixxxFighter.buttonPressDuringChannelSelect === false) {
        switchChannel(buttonChannelIndex);
      }
      if (hasSelectedNonActiveChannel() === false) {
        unregisterButtonPressDuringChannelSelect();
      }
    }
  }
}

MixxxFighter.channelOneButton = channelButton(0);
MixxxFighter.channelTwoButton = channelButton(1);
MixxxFighter.channelThreeButton = channelButton(2);
MixxxFighter.channelFourButton = channelButton(3);


MixxxFighter.leftButton = function(channel, control, value, status, group) {
  value == BUTTON_RELEASED && engine.setValue('[Library]', MixxxFighter.shift === false ? 'MoveFocusForward' : 'MoveFocusBackward', 1);
}

MixxxFighter.upButton = function(channel, control, value, status, group) {
  value == BUTTON_RELEASED && engine.setValue('[Library]', 'MoveUp', 1);
}

MixxxFighter.downButton = function(channel, control, value, status, group) {
  value == BUTTON_RELEASED && engine.setValue('[Library]', 'MoveDown', 1);
}

MixxxFighter.rightButton = Button(function(channel, control, value, status, group) {
  value == BUTTON_RELEASED && engineSetValueForSelectedChannels('LoadSelectedTrack', 1);
})

MixxxFighter.shiftButton = function(channel, control, value, status, group) {
  if (value == BUTTON_PRESSED) {
    MixxxFighter.shift = true;
    DBG('Shift pressed');
  } else {
    MixxxFighter.shift = false;
    DBG('Shift released');
  }
}

MixxxFighter.playButton = Button(function(_channel, control, value, status, group) {
  if (value == BUTTON_PRESSED) {
    if (MixxxFighter.shift === true) {
      engineSetValueForSelectedChannels('cue_default', 1);
    } else {
      DBG("Play " + String(value));
      forEachSelectedChannel(function (i, group) {
        engine.setValue(group, 'play', !(engine.getValue(group, 'play'))); 
      });
    }
  } else {
    if (MixxxFighter.shift === true) {
      engineSetValueForSelectedChannels('cue_default', 0);
    } 
  }
});

MixxxFighter.slowerButton = Button(function(_channel, control, value, status, group) {
  if (value == BUTTON_PRESSED) {
    if (MixxxFighter.shift) {
      engineSetValueForSelectedChannels('rate_temp_down_small', 1);
    } else {
      engineSetValueForSelectedChannels('rate_temp_down', 1);
    }
  } else { 
    forEachSelectedChannel(function (i, group) {
      if (engine.getValue(group, 'rate_temp_down') === 1) {
        engine.setValue(group, 'rate_temp_down', 0);
      }
      if (engine.getValue(group, 'rate_temp_down_small') === 1) {
        engine.setValue(group, 'rate_temp_down_small', 0);
      }
      if (engine.getValue(group, 'play') !== 0) {
        engine.setValue(group, 'beats_translate_match_alignment', 1);
      }
    });
  }
});

MixxxFighter.fasterButton = Button(function(_channel, control, value, status, group) {
  if (value == BUTTON_PRESSED) {
    if (MixxxFighter.shift) {
      engineSetValueForSelectedChannels('rate_temp_up_small', 1);
    } else {
      engineSetValueForSelectedChannels('rate_temp_up', 1);
    }
  } else { 
    forEachSelectedChannel(function (i, group) {
      if (engine.getValue(group, 'rate_temp_up') === 1) {
        engine.setValue(group, 'rate_temp_up', 0);
      }
      if (engine.getValue(group, 'rate_temp_up_small') === 1) {
        engine.setValue(group, 'rate_temp_up_small', 0);
      }
      if (engine.getValue(group, 'play') !== 0) {
        engine.setValue(group, 'beats_translate_match_alignment', 1);
      }
    })
  }
});

MixxxFighter.emergencyLoopButton = Button(function(_channel, control, value, status, group) {
  if (value == BUTTON_RELEASED) {
    if (MixxxFighter.shift) {
      engineSetValueForSelectedChannels('reloop_toggle', 1);
    } else {
      forEachSelectedChannel(function (i, group) {
        if (engine.getValue(group, 'beatloop_activate') == 1) {
          engine.setValue(group, 'reloop_toggle', 1);
        }
        engine.setValue(group, 'beatloop_activate', 1);
      });
    } 
  } 
});

function hotcuePageTwo(hotcueNumber, value) {
  if (MixxxFighter.shift) {
    value == BUTTON_RELEASED && engineSetValueForSelectedChannels('hotcue_' + hotcueNumber + '_clear', 1);
  } else {
    value == BUTTON_RELEASED && engineSetValueForSelectedChannels('hotcue_' + hotcueNumber + '_activate', 1);
  }
}
function hotcuePageThree(hotcueNumber, value) {
  if (MixxxFighter.shift) {
    value == BUTTON_RELEASED && engineSetValueForSelectedChannels('hotcue_' + hotcueNumber + '_clear', 1);
  } else {
    value == BUTTON_PRESSED && engineSetValueForSelectedChannels('hotcue_' + hotcueNumber + '_activate', 1); 
    value == BUTTON_RELEASED && engineSetValueForSelectedChannels('hotcue_' + hotcueNumber + '_activate', 0); 
  }
}

MixxxFighter.modifierOneButton = Button(function(_channel, control, value, status, group) {
  if (MixxxFighter.modifierPage == 0) {
    if (MixxxFighter.shift) {
      value == BUTTON_RELEASED && engineSetValueForSelectedChannels('beatjump_size', Math.floor(engine.getValue(MixxxFighter.group, 'beatjump_size') / 2));
    } else {
      value == BUTTON_RELEASED && engineSetValueForSelectedChannels('beatjump_backward', 1);
    }
  } else if (MixxxFighter.modifierPage == 1) {
    hotcuePageTwo(1, value);
  } else if (MixxxFighter.modifierPage == 2) {
    hotcuePageThree(1, value);
  } else if (MixxxFighter.modifierPage == 3) {
  }
});


MixxxFighter.modifierTwoButton = Button(function(_channel, control, value, status, group) {
  if (MixxxFighter.modifierPage == 0) {
    if (MixxxFighter.shift) {
      value == BUTTON_RELEASED && forEachSelectedChannel(function (i, group) {
	engine.setValue(group, 'beatjump_size', engine.getValue(group, 'beatjump_size') - 1);
      })
    } else {
      value == BUTTON_RELEASED && engineSetValueForSelectedChannels('beatjump_1_backward', 1);
    }
  } else if (MixxxFighter.modifierPage == 1) {
    hotcuePageTwo(2, value);
  } else if (MixxxFighter.modifierPage == 2) {
    hotcuePageThree(2, value);
  } else if (MixxxFighter.modifierPage == 3) {
  }
});

MixxxFighter.modifierThreeButton = Button(function(_channel, control, value, status, group) {
  if (MixxxFighter.modifierPage == 0) {
    if (MixxxFighter.shift) {
      value == BUTTON_RELEASED && forEachSelectedChannel(function (i, group) {
        engine.setValue(group, 'beatjump_size', engine.getValue(group, 'beatjump_size') + 1);
      })
    } else {
      value == BUTTON_RELEASED && engineSetValueForSelectedChannels('beatjump_1_forward', 1);
    }
  } else if (MixxxFighter.modifierPage == 1) {
    hotcuePageTwo(3, value);
  } else if (MixxxFighter.modifierPage == 2) {
    hotcuePageThree(3, value);
  } else if (MixxxFighter.modifierPage == 3) {
  }
});

MixxxFighter.modifierFourButton = Button(function(_channel, control, value, status, group) {
  if (MixxxFighter.modifierPage == 0) {
    if (MixxxFighter.shift) {
      value == BUTTON_RELEASED && forEachSelectedChannel(function (i, group) {
        engine.setValue(group, 'beatjump_size', Math.floor(engine.getValue(group, 'beatjump_size') * 2));
      })
    } else {
      value == BUTTON_RELEASED && engineSetValueForSelectedChannels('beatjump_forward', 1);
    }
  } else if (MixxxFighter.modifierPage == 1) {
    hotcuePageTwo(4, value);
  } else if (MixxxFighter.modifierPage == 2) {
    hotcuePageThree(4, value);
  } else if (MixxxFighter.modifierPage == 3) {
  }
});

MixxxFighter.modifierFiveButton = Button(function(_channel, control, value, status, group) {
  if (MixxxFighter.modifierPage == 0) {
    if (MixxxFighter.shift) {
      value == BUTTON_RELEASED && forEachSelectedChannel(function (i, group) {
        engine.setValue(group, 'beatloop_size', Math.floor(engine.getValue(group, 'beatloop_size') / 2));
      })
    } else {
      value == BUTTON_RELEASED && engineSetValueForSelectedChannels('beatloop_1_activate', 1);
    }
  } else if (MixxxFighter.modifierPage == 1) {
    hotcuePageTwo(5, value);
  } else if (MixxxFighter.modifierPage == 2) {
    hotcuePageThree(5, value);
  } else if (MixxxFighter.modifierPage == 3) {
  }
});

MixxxFighter.modifierSixButton = Button(function(_channel, control, value, status, group) {
  if (MixxxFighter.modifierPage == 0) {
    if (MixxxFighter.shift) {
      value == BUTTON_RELEASED && forEachSelectedChannel(function (i, group) {
        engine.setValue(group, 'beatloop_size', engine.getValue(group, 'beatloop_size') - 1);
      })
    } else {
      value == BUTTON_RELEASED && engineSetValueForSelectedChannels('beatloop_4_activate', 1);
    }
  } else if (MixxxFighter.modifierPage == 1) {
    hotcuePageTwo(6, value);
  } else if (MixxxFighter.modifierPage == 2) {
    hotcuePageThree(6, value);
  } else if (MixxxFighter.modifierPage == 3) {
  }
});

MixxxFighter.modifierSevenButton = Button(function(_channel, control, value, status, group) {
  if (MixxxFighter.modifierPage == 0) {
    if (MixxxFighter.shift) {
      value == BUTTON_RELEASED && forEachSelectedChannel(function (i, group) {
        engine.setValue(group, 'beatloop_size', engine.getValue(group, 'beatloop_size') + 1);
      })
    } else {
      value == BUTTON_RELEASED && engineSetValueForSelectedChannels('beatloop_8_activate', 1);
    }
  } else if (MixxxFighter.modifierPage == 1) {
    hotcuePageTwo(7, value);
  } else if (MixxxFighter.modifierPage == 2) {
    hotcuePageThree(7, value);
  } else if (MixxxFighter.modifierPage == 3) {
  }
});

MixxxFighter.modifierEightButton = Button(function(_channel, control, value, status, group) {
  if (MixxxFighter.modifierPage == 0) {
    if (MixxxFighter.shift) {
      value == BUTTON_RELEASED && forEachSelectedChannel(function (i, group) {
        engine.setValue(group, 'beatloop_size', Math.floor(engine.getValue(group, 'beatloop_size') * 2));
      })
    } else {
      value == BUTTON_RELEASED && engineSetValueForSelectedChannels('beatloop_16_activate', 1);
    }
  } else if (MixxxFighter.modifierPage == 1) {
    hotcuePageTwo(8, value);
  } else if (MixxxFighter.modifierPage == 2) {
    hotcuePageThree(8, value);
  } else if (MixxxFighter.modifierPage == 3) {
  }
});

function switchModifierPage(pageIndex) {
  var previousPage = MixxxFighter.modifierPage;
  midi.sendShortMsg(0x80, 0x32 + previousPage, 0x1);
  MixxxFighter.modifierPage = pageIndex;
  midi.sendShortMsg(0x90, 0x32 + pageIndex, 0x1);
  DBG("Switched modifier page from " + String(previousPage) + " to " + String(pageIndex));
}

function switchModifierPageButton(pageIndex) {
  return function(channel, control, value, status, group) {
    if (value != BUTTON_PRESSED) return;
    switchModifierPage(pageIndex);
  }
}

MixxxFighter.modifierPageOneButton   = switchModifierPageButton(0)
MixxxFighter.modifierPageTwoButton   = switchModifierPageButton(1)
MixxxFighter.modifierPageThreeButton = switchModifierPageButton(2)
MixxxFighter.modifierPageFourButton  = switchModifierPageButton(3)

MixxxFighter.shutdown = function() {
  DBG("Goodbye from MixxxFighter!");
  midi.sendShortMsg(0x80, MixxxFighter.selectedChannel, 0x1);
  midi.sendShortMsg(0x80, 0x32 + MixxxFighter.modifierPage, 0x1);
}
