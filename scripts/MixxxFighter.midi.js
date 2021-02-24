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
}

function switchChannel(channel) {
  let previousChannel = MixxxFighter.channel
  midi.sendShortMsg(0x80, previousChannel, 0x1) 
  MixxxFighter.channel = channel
  MixxxFighter.group = '[Channel' + String(MixxxFighter.channel + 1) + ']'
  midi.sendShortMsg(0x90, channel, 0x1) 
  DBG("Switched from " + String(previousChannel) + " to " + String(channel))
}

MixxxFighter.channelOneButton = function(channel, control, value, status, group) {
  value == BUTTON_RELEASED && switchChannel(0)
}

MixxxFighter.channelTwoButton = function(channel, control, value, status, group) {
  value == BUTTON_RELEASED && switchChannel(1)
}

MixxxFighter.channelThreeButton = function(channel, control, value, status, group) {
  value == BUTTON_RELEASED && switchChannel(2)
}

MixxxFighter.channelFourButton = function(channel, control, value, status, group) {
  value == BUTTON_RELEASED && switchChannel(3)
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
  engine.setValue('[Library]', 'GoToItem', 1);
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
      engineSetValue('cue_default', 1) 
    } else {
      let value = engineGetValue('play');
      DBG("Play " + String(value));
      engineSetValue('play', !(engine.getValue(MixxxFighter.group, 'play'))); 
    }
  } else {
    if (MixxxFighter.shift === true) {
      engineSetValue('cue_default', 0);
    } 
  }
}

function switchModifierPage(pageIndex) {
  let previousPage = MixxxFighter.modifierPage
  midi.sendShortMsg(0x80, 0x32 + previousPage, 0x1) 
  MixxxFighter.modifierPage = pageIndex
  midi.sendShortMsg(0x90, 0x32 + pageIndex, 0x1) 
  DBG("Switched modifier page from " + String(previousPage) + " to " + String(pageIndex))
}

MixxxFighter.modifierPageOneButton = function (_channel, control, value, status, group) {
  value == BUTTON_PRESSED && switchModifierPage(0)
}
MixxxFighter.modifierPageTwoButton = function (_channel, control, value, status, group) {
  value == BUTTON_PRESSED && switchModifierPage(1)
}
MixxxFighter.modifierPageThreeButton = function (_channel, control, value, status, group) {
  value == BUTTON_PRESSED && switchModifierPage(2)
}
MixxxFighter.modifierPageFourButton = function (_channel, control, value, status, group) {
  value == BUTTON_PRESSED && switchModifierPage(3)
}

MixxxFighter.syncButton = function(channel, control, value, status, group) {
  if (value == BUTTON_PRESSED) {
  } else {
    if (MixxxFighter.shift === true) {
      engine.setValue(group, 'quantize', !engine.getValue(group, 'quantize'))
    } else {
      engine.setValue(group, 'beatsync_tempo', !engine.getValue(group, 'beatsync_tempo'))
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
      engine.setValue(group, 'rate_temp_down', 0)
      engine.setValue(group, 'beats_translate_match_alignment', 1)
    } else {
      engine.setValue(group, 'reloop_toggle', 1)
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
      engine.setValue(group, 'rate_temp_up', 0)
      engine.setValue(group, 'beats_translate_match_alignment', 1)
    } else {
      engine.setValue(group, 'beatloop_activate', 1)
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
}
