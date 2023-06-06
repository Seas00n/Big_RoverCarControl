// Auto-generated. Do not edit!

// (in-package ecm_bridge.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class driverCmd {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.enable = null;
      this.opMode = null;
      this.kp = null;
      this.kd = null;
      this.q = null;
      this.qd = null;
      this.cur = null;
    }
    else {
      if (initObj.hasOwnProperty('enable')) {
        this.enable = initObj.enable
      }
      else {
        this.enable = [];
      }
      if (initObj.hasOwnProperty('opMode')) {
        this.opMode = initObj.opMode
      }
      else {
        this.opMode = [];
      }
      if (initObj.hasOwnProperty('kp')) {
        this.kp = initObj.kp
      }
      else {
        this.kp = [];
      }
      if (initObj.hasOwnProperty('kd')) {
        this.kd = initObj.kd
      }
      else {
        this.kd = [];
      }
      if (initObj.hasOwnProperty('q')) {
        this.q = initObj.q
      }
      else {
        this.q = [];
      }
      if (initObj.hasOwnProperty('qd')) {
        this.qd = initObj.qd
      }
      else {
        this.qd = [];
      }
      if (initObj.hasOwnProperty('cur')) {
        this.cur = initObj.cur
      }
      else {
        this.cur = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type driverCmd
    // Serialize message field [enable]
    bufferOffset = _arraySerializer.uint8(obj.enable, buffer, bufferOffset, null);
    // Serialize message field [opMode]
    bufferOffset = _arraySerializer.uint8(obj.opMode, buffer, bufferOffset, null);
    // Serialize message field [kp]
    bufferOffset = _arraySerializer.float64(obj.kp, buffer, bufferOffset, null);
    // Serialize message field [kd]
    bufferOffset = _arraySerializer.float64(obj.kd, buffer, bufferOffset, null);
    // Serialize message field [q]
    bufferOffset = _arraySerializer.float64(obj.q, buffer, bufferOffset, null);
    // Serialize message field [qd]
    bufferOffset = _arraySerializer.float64(obj.qd, buffer, bufferOffset, null);
    // Serialize message field [cur]
    bufferOffset = _arraySerializer.float64(obj.cur, buffer, bufferOffset, null);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type driverCmd
    let len;
    let data = new driverCmd(null);
    // Deserialize message field [enable]
    data.enable = _arrayDeserializer.uint8(buffer, bufferOffset, null)
    // Deserialize message field [opMode]
    data.opMode = _arrayDeserializer.uint8(buffer, bufferOffset, null)
    // Deserialize message field [kp]
    data.kp = _arrayDeserializer.float64(buffer, bufferOffset, null)
    // Deserialize message field [kd]
    data.kd = _arrayDeserializer.float64(buffer, bufferOffset, null)
    // Deserialize message field [q]
    data.q = _arrayDeserializer.float64(buffer, bufferOffset, null)
    // Deserialize message field [qd]
    data.qd = _arrayDeserializer.float64(buffer, bufferOffset, null)
    // Deserialize message field [cur]
    data.cur = _arrayDeserializer.float64(buffer, bufferOffset, null)
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += object.enable.length;
    length += object.opMode.length;
    length += 8 * object.kp.length;
    length += 8 * object.kd.length;
    length += 8 * object.q.length;
    length += 8 * object.qd.length;
    length += 8 * object.cur.length;
    return length + 28;
  }

  static datatype() {
    // Returns string type for a message object
    return 'ecm_bridge/driverCmd';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '4ac7643f368fa7335ff4aa8b3f789c47';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    uint8[] enable
    uint8[] opMode
    float64[] kp
    float64[] kd
    float64[] q
    float64[] qd
    float64[] cur
    
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new driverCmd(null);
    if (msg.enable !== undefined) {
      resolved.enable = msg.enable;
    }
    else {
      resolved.enable = []
    }

    if (msg.opMode !== undefined) {
      resolved.opMode = msg.opMode;
    }
    else {
      resolved.opMode = []
    }

    if (msg.kp !== undefined) {
      resolved.kp = msg.kp;
    }
    else {
      resolved.kp = []
    }

    if (msg.kd !== undefined) {
      resolved.kd = msg.kd;
    }
    else {
      resolved.kd = []
    }

    if (msg.q !== undefined) {
      resolved.q = msg.q;
    }
    else {
      resolved.q = []
    }

    if (msg.qd !== undefined) {
      resolved.qd = msg.qd;
    }
    else {
      resolved.qd = []
    }

    if (msg.cur !== undefined) {
      resolved.cur = msg.cur;
    }
    else {
      resolved.cur = []
    }

    return resolved;
    }
};

module.exports = driverCmd;
