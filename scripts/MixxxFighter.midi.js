const BUTTON_PRESSED = 127;
const BUTTON_RELEASED = 0;


function DBG(str) {
  print("[MixxxFighter] " + str);
}

function engineSetValue(key, value) {
  engine.setValue(MixxxFighter.group, key, value)
}

function engineGetValue(key) {
  DBG(MixxxFighter.group)
  engine.getValue(MixxxFighter.group, key)
}



var MixxxFighter = {};
MixxxFighter.channel = 0
MixxxFighter.modifierPage = 0
MixxxFighter.shift = false
MixxxFighter.focusSidebar = false

MixxxFighter.init = function(id, debugging) {
  DBG("Hello from MixxxFighter!");
  switchChannel(MixxxFighter.channel);
  switchModifierPage(MixxxFighter.modifierPage);
  for (var i = 1; i < 5; i++) {
    var group = '[Channel' + String(i) + ']';
    engine.setValue(group, 'quantize', 1);
    engine.setValue(group, 'sync_enabled', 1);
  }
}

function switchChannel(channel) {
  var previousChannel = MixxxFighter.channel;
  midi.sendShortMsg(0x80, previousChannel, 0x1); 
  MixxxFighter.channel = channel;
  MixxxFighter.group = '[Channel' + String(MixxxFighter.channel + 1) + ']';
  midi.sendShortMsg(0x90, channel, 0x1);
  DBG("Switched from " + String(previousChannel) + " to " + String(channel));
}

MixxxFighter.channelOneButton = function(channel, control, value, status, group) {
  if (value == BUTTON_RELEASED) {
    if (MixxxFighter.shift) {
      engine.setValue('[Channel1]', 'CloneFromDeck', MixxxFighter.channel + 1);
    } else {
      switchChannel(0);
    }
  }
}

MixxxFighter.channelTwoButton = function(channel, control, value, status, group) {
  if (value == BUTTON_RELEASED) {
    if (MixxxFighter.shift) {
      engine.setValue('[Channel2]', 'CloneFromDeck', MixxxFighter.channel + 1);
    } else {
      switchChannel(1);
    }
  }
}

MixxxFighter.channelThreeButton = function(channel, control, value, status, group) {
  if (value == BUTTON_RELEASED) {
    if (MixxxFighter.shift) {
      engine.setValue('[Channel3]', 'CloneFromDeck', MixxxFighter.channel + 1);
    } else {
      switchChannel(2);
    }
  }
}

MixxxFighter.channelFourButton = function(channel, control, value, status, group) {
  if (value == BUTTON_RELEASED) {
    if (MixxxFighter.shift) {
      engine.setValue('[Channel4]', 'CloneFromDeck', MixxxFighter.channel + 1);
    } else {
      switchChannel(3);
    }
  }
}



MixxxFighter.leftButton = function(channel, control, value, status, group) {
  if (value != BUTTON_RELEASED) return;
  
  engine.setValue('[Library]', MixxxFighter.shift === false ? 'MoveFocusForward' : 'MoveFocusBackward', 1);
}

MixxxFighter.upButton = function(channel, control, value, status, group) {
  value == BUTTON_RELEASED && engine.setValue('[Library]', 'MoveUp', 1);
}

MixxxFighter.downButton = function(channel, control, value, status, group) {
  value == BUTTON_RELEASED && engine.setValue('[Library]', 'MoveDown', 1);
}

MixxxFighter.rightButton = function(channel, control, value, status, group) {
  if (value != BUTTON_RELEASED) return;
  engineSetValue('LoadSelectedTrack', 1);
}

MixxxFighter.shiftButton = function(channel, control, value, status, group) {
  if (value == BUTTON_PRESSED) {
    MixxxFighter.shift = true;
    DBG('Shift pressed');
  } else {
    MixxxFighter.shift = false;
    DBG('Shift released');
  }
}

MixxxFighter.playButton = function(_channel, control, value, status, group) {
  if (value == BUTTON_PRESSED) {
    if (MixxxFighter.shift === true) {
      engineSetValue('cue_default', 1);
    } else {
      var value = engineGetValue('play');
      DBG("Play " + String(value));
      engineSetValue('play', !(engine.getValue(MixxxFighter.group, 'play'))); 
    }
  } else {
    if (MixxxFighter.shift === true) {
      engineSetValue('cue_default', 0);
    } 
  }
}

MixxxFighter.slowerButton = function(_channel, control, value, status, group) {
  if (value == BUTTON_PRESSED) {
    if (MixxxFighter.shift) {
      engineSetValue('rate_temp_down_small', 1);
    } else {
      engineSetValue('rate_temp_down', 1);
    }
  } else { 
    if (engine.getValue(MixxxFighter.group, 'rate_temp_down') === 1) {
      engineSetValue('rate_temp_down', 0);
    }
    if (engine.getValue(MixxxFighter.group, 'rate_temp_down_small') === 1) {
      engineSetValue('rate_temp_down_small', 0);
    }
    if (engine.getValue(MixxxFighter.group, 'play') !== 0) {
      engineSetValue('beats_translate_match_alignment', 1);
    }
  }
}

MixxxFighter.fasterButton = function(_channel, control, value, status, group) {
  if (value == BUTTON_PRESSED) {
    if (MixxxFighter.shift) {
      engineSetValue('rate_temp_up_small', 1);
    } else {
      engineSetValue('rate_temp_up', 1);
    }
  } else { 
    if (engine.getValue(MixxxFighter.group, 'rate_temp_up') === 1) {
      engineSetValue('rate_temp_up', 0);
    }
    if (engine.getValue(MixxxFighter.group, 'rate_temp_up_small') === 1) {
      engineSetValue('rate_temp_up_small', 0);
    }
    if (engine.getValue(MixxxFighter.group, 'play') !== 0) {
      engineSetValue('beats_translate_match_alignment', 1);
    }
  }
}

MixxxFighter.emergencyLoopButton = function(_channel, control, value, status, group) {
  if (value == BUTTON_RELEASED) {
    if (MixxxFighter.shift) {
      engineSetValue('reloop_toggle', 1);
    } else {
      if (engine.getValue(MixxxFighter.group, 'beatloop_activate') == 1) {
        engineSetValue('reloop_toggle', 1);
      }
      engineSetValue('beatloop_activate', 1);
    } 
  } 
}

function hotcuePageTwo(hotcueNumber, value) {
  if (MixxxFighter.shift) {
    value == BUTTON_RELEASED && engineSetValue('hotcue_' + hotcueNumber + '_clear', 1);
  } else {
    value == BUTTON_RELEASED && engineSetValue('hotcue_' + hotcueNumber + '_activate', 1);
  }
}
function hotcuePageThree(hotcueNumber, value) {
  if (MixxxFighter.shift) {
    value == BUTTON_RELEASED && engineSetValue('hotcue_' + hotcueNumber + '_clear', 1);
  } else {
    value == BUTTON_PRESSED && engineSetValue('hotcue_' + hotcueNumber + '_activate', 1); 
    value == BUTTON_RELEASED && engineSetValue('hotcue_' + hotcueNumber + '_activate', 0); 
  }
}

MixxxFighter.modifierOneButton = function(_channel, control, value, status, group) {
  if (MixxxFighter.modifierPage == 0) {
    if (MixxxFighter.shift) {
      value == BUTTON_RELEASED && engineSetValue('beatjump_size', Math.floor(engine.getValue(MixxxFighter.group, 'beatjump_size') / 2));
    } else {
      value == BUTTON_RELEASED && engineSetValue('beatjump_backward', 1);
    }
  } else if (MixxxFighter.modifierPage == 1) {
    hotcuePageTwo(1, value);
  } else if (MixxxFighter.modifierPage == 2) {
    hotcuePageThree(1, value);
  } else if (MixxxFighter.modifierPage == 3) {
  }
}


MixxxFighter.modifierTwoButton = function(_channel, control, value, status, group) {
  if (MixxxFighter.modifierPage == 0) {
    if (MixxxFighter.shift) {
      value == BUTTON_RELEASED && engineSetValue('beatjump_size', engine.getValue(MixxxFighter.group, 'beatjump_size') - 1);
    } else {
      value == BUTTON_RELEASED && engineSetValue('beatjump_1_backward', 1);
    }
  } else if (MixxxFighter.modifierPage == 1) {
    hotcuePageTwo(2, value);
  } else if (MixxxFighter.modifierPage == 2) {
    hotcuePageThree(2, value);
  } else if (MixxxFighter.modifierPage == 3) {
  }
}

MixxxFighter.modifierThreeButton = function(_channel, control, value, status, group) {
  if (MixxxFighter.modifierPage == 0) {
    if (MixxxFighter.shift) {
      value == BUTTON_RELEASED && engineSetValue('beatjump_size', engine.getValue(MixxxFighter.group, 'beatjump_size') + 1);
    } else {
      value == BUTTON_RELEASED && engineSetValue('beatjump_1_forward', 1);
    }
  } else if (MixxxFighter.modifierPage == 1) {
    hotcuePageTwo(3, value);
  } else if (MixxxFighter.modifierPage == 2) {
    hotcuePageThree(3, value);
  } else if (MixxxFighter.modifierPage == 3) {
  }
}

MixxxFighter.modifierFourButton = function(_channel, control, value, status, group) {
  if (MixxxFighter.modifierPage == 0) {
    if (MixxxFighter.shift) {
      value == BUTTON_RELEASED && engineSetValue('beatjump_size', Math.floor(engine.getValue(MixxxFighter.group, 'beatjump_size') * 2));
    } else {
      value == BUTTON_RELEASED && engineSetValue('beatjump_forward', 1);
    }
  } else if (MixxxFighter.modifierPage == 1) {
    hotcuePageTwo(4, value);
  } else if (MixxxFighter.modifierPage == 2) {
    hotcuePageThree(4, value);
  } else if (MixxxFighter.modifierPage == 3) {
  }
}

MixxxFighter.modifierFiveButton = function(_channel, control, value, status, group) {
  if (MixxxFighter.modifierPage == 0) {
    if (MixxxFighter.shift) {
      value == BUTTON_RELEASED && engineSetValue('beatloop_size', Math.floor(engine.getValue(MixxxFighter.group, 'beatloop_size') / 2));
    } else {
      value == BUTTON_RELEASED && engineSetValue('beatloop_1_activate', 1);
    }
  } else if (MixxxFighter.modifierPage == 1) {
    hotcuePageTwo(5, value);
  } else if (MixxxFighter.modifierPage == 2) {
    hotcuePageThree(5, value);
  } else if (MixxxFighter.modifierPage == 3) {
  }
}

MixxxFighter.modifierSixButton = function(_channel, control, value, status, group) {
  if (MixxxFighter.modifierPage == 0) {
    if (MixxxFighter.shift) {
      value == BUTTON_RELEASED && engineSetValue('beatloop_size', engine.getValue(MixxxFighter.group, 'beatloop_size') - 1);
    } else {
      value == BUTTON_RELEASED && engineSetValue('beatloop_4_activate', 1);
    }
  } else if (MixxxFighter.modifierPage == 1) {
    hotcuePageTwo(6, value);
  } else if (MixxxFighter.modifierPage == 2) {
    hotcuePageThree(6, value);
  } else if (MixxxFighter.modifierPage == 3) {
  }
}

MixxxFighter.modifierSevenButton = function(_channel, control, value, status, group) {
  if (MixxxFighter.modifierPage == 0) {
    if (MixxxFighter.shift) {
      value == BUTTON_RELEASED && engineSetValue('beatloop_size', engine.getValue(MixxxFighter.group, 'beatloop_size') + 1);
    } else {
      value == BUTTON_RELEASED && engineSetValue('beatloop_8_activate', 1);
    }
  } else if (MixxxFighter.modifierPage == 1) {
    hotcuePageTwo(7, value);
  } else if (MixxxFighter.modifierPage == 2) {
    hotcuePageThree(7, value);
  } else if (MixxxFighter.modifierPage == 3) {
  }
}

MixxxFighter.modifierEightButton = function(_channel, control, value, status, group) {
  if (MixxxFighter.modifierPage == 0) {
    if (MixxxFighter.shift) {
      value == BUTTON_RELEASED && engineSetValue('beatloop_size', Math.floor(engine.getValue(MixxxFighter.group, 'beatloop_size') * 2));
    } else {
      value == BUTTON_RELEASED && engineSetValue('beatloop_16_activate', 1);
    }
  } else if (MixxxFighter.modifierPage == 1) {
    hotcuePageTwo(8, value);
  } else if (MixxxFighter.modifierPage == 2) {
    hotcuePageThree(8, value);
  } else if (MixxxFighter.modifierPage == 3) {
  }
}
 


function switchModifierPage(pageIndex) {
  var previousPage = MixxxFighter.modifierPage;
  midi.sendShortMsg(0x80, 0x32 + previousPage, 0x1);
  MixxxFighter.modifierPage = pageIndex;
  midi.sendShortMsg(0x90, 0x32 + pageIndex, 0x1);
  DBG("Switched modifier page from " + String(previousPage) + " to " + String(pageIndex));
}

MixxxFighter.modifierPageOneButton = function (_channel, control, value, status, group) {
  value == BUTTON_PRESSED && switchModifierPage(0);
}
MixxxFighter.modifierPageTwoButton = function (_channel, control, value, status, group) {
  value == BUTTON_PRESSED && switchModifierPage(1);
}
MixxxFighter.modifierPageThreeButton = function (_channel, control, value, status, group) {
  value == BUTTON_PRESSED && switchModifierPage(2);
}
MixxxFighter.modifierPageFourButton = function (_channel, control, value, status, group) {
  value == BUTTON_PRESSED && switchModifierPage(3);
}

MixxxFighter.syncButton = function(channel, control, value, status, group) {
  if (value == BUTTON_PRESSED) {
  } else {
    if (MixxxFighter.shift === true) {
      engine.setValue(group, 'quantize', !engine.getValue(group, 'quantize'));
    } else {
      engine.setValue(group, 'beatsync_tempo', !engine.getValue(group, 'beatsync_tempo'));
    }
  }
}


MixxxFighter.beatJumpButton = function(channel, control, value, status, group) {
  if (value == BUTTON_PRESSED) {
    DBG('beatJumpButton: pressed');

    if (MixxxFighter.shift === true) {
      DBG('beatJumpButton: shift is pressed, slowing down ' + group);
      engine.setValue(group, 'rate_temp_down', 1);
    }
  } else {
    DBG('beatJumpButton: released');
    if (engine.getValue(group, 'rate_temp_down') === 1) {
      engine.setValue(group, 'rate_temp_down', 0);
      engine.setValue(group, 'beats_translate_match_alignment', 1);
    } else {
      engine.setValue(group, 'reloop_toggle', 1);
    }
  }
}

MixxxFighter.loopButton = function(channel, control, value, status, group) {
  if (value == BUTTON_PRESSED) {
    DBG('loopButton: pressed');
    if (MixxxFighter.shift === true) {
      DBG('loopButton: shift is pressed, slowing down ' + group);
      engine.setValue(group, 'rate_temp_up', 1);
    }
  } else {
    DBG('loopButton: released');
    if (engine.getValue(group, 'rate_temp_up') === 1) {
      engine.setValue(group, 'rate_temp_up', 0);
      engine.setValue(group, 'beats_translate_match_alignment', 1);
    } else {
      engine.setValue(group, 'beatloop_activate', 1);
    }
  }
}

MixxxFighter.loopEncoder = function(channel, control, value, status, group) {
  var loop_size = engine.getValue(group, 'beatloop_size');
  if (isRotaryEncoderCW(value) == true) {
    print("loopEncoder: turned CW");
    loop_size = loop_size * 2;
  } else {
    print("loopEncoder: turned CCW");
    loop_size = loop_size / 2;
  }
  engine.setValue(group, 'beatjump_size', loop_size);
  engine.setValue(group, 'beatloop_size', loop_size);
}

MixxxFighter.shutdown = function() {
  DBG("Goodbye from MixxxFighter!");
  midi.sendShortMsg(0x80, MixxxFighter.channel, 0x1);
  midi.sendShortMsg(0x80, 0x32 + MixxxFighter.modifierPage, 0x1);
}
