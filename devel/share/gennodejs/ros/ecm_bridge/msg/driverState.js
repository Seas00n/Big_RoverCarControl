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

class driverState {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.sw = null;
      this.error = null;
      this.opMode = null;
      this.complement = null;
      this.q = null;
      this.qd = null;
      this.cur = null;
      this.dc_v = null;
    }
    else {
      if (initObj.hasOwnProperty('sw')) {
        this.sw = initObj.sw
      }
      else {
        this.sw = [];
      }
      if (initObj.hasOwnProperty('error')) {
        this.error = initObj.error
      }
      else {
        this.error = [];
      }
      if (initObj.hasOwnProperty('opMode')) {
        this.opMode = initObj.opMode
      }
      else {
        this.opMode = [];
      }
      if (initObj.hasOwnProperty('complement')) {
        this.complement = initObj.complement
      }
      else {
        this.complement = [];
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
      if (initObj.hasOwnProperty('dc_v')) {
        this.dc_v = initObj.dc_v
      }
      else {
        this.dc_v = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type driverState
    // Serialize message field [sw]
    bufferOffset = _arraySerializer.uint16(obj.sw, buffer, bufferOffset, null);
    // Serialize message field [error]
    bufferOffset = _arraySerializer.uint16(obj.error, buffer, bufferOffset, null);
    // Serialize message field [opMode]
    bufferOffset = _arraySerializer.uint8(obj.opMode, buffer, bufferOffset, null);
    // Serialize message field [complement]
    bufferOffset = _arraySerializer.uint8(obj.complement, buffer, bufferOffset, null);
    // Serialize message field [q]
    bufferOffset = _arraySerializer.float64(obj.q, buffer, bufferOffset, null);
    // Serialize message field [qd]
    bufferOffset = _arraySerializer.float64(obj.qd, buffer, bufferOffset, null);
    // Serialize message field [cur]
    bufferOffset = _arraySerializer.float64(obj.cur, buffer, bufferOffset, null);
    // Serialize message field [dc_v]
    bufferOffset = _arraySerializer.float64(obj.dc_v, buffer, bufferOffset, null);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type driverState
    let len;
    let data = new driverState(null);
    // Deserialize message field [sw]
    data.sw = _arrayDeserializer.uint16(buffer, bufferOffset, null)
    // Deserialize message field [error]
    data.error = _arrayDeserializer.uint16(buffer, bufferOffset, null)
    // Deserialize message field [opMode]
    data.opMode = _arrayDeserializer.uint8(buffer, bufferOffset, null)
    // Deserialize message field [complement]
    data.complement = _arrayDeserializer.uint8(buffer, bufferOffset, null)
    // Deserialize message field [q]
    data.q = _arrayDeserializer.float64(buffer, bufferOffset, null)
    // Deserialize message field [qd]
    data.qd = _arrayDeserializer.float64(buffer, bufferOffset, null)
    // Deserialize message field [cur]
    data.cur = _arrayDeserializer.float64(buffer, bufferOffset, null)
    // Deserialize message field [dc_v]
    data.dc_v = _arrayDeserializer.float64(buffer, bufferOffset, null)
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += 2 * object.sw.length;
    length += 2 * object.error.length;
    length += object.opMode.length;
    length += object.complement.length;
    length += 8 * object.q.length;
    length += 8 * object.qd.length;
    length += 8 * object.cur.length;
    length += 8 * object.dc_v.length;
    return length + 32;
  }

  static datatype() {
    // Returns string type for a message object
    return 'ecm_bridge/driverState';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '2deea21f9bf25c298b3269d351909d41';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    uint16[] sw
    uint16[] error
    uint8[] opMode
    uint8[] complement
    float64[] q
    float64[] qd
    float64[] cur
    float64[] dc_v
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new driverState(null);
    if (msg.sw !== undefined) {
      resolved.sw = msg.sw;
    }
    else {
      resolved.sw = []
    }

    if (msg.error !== undefined) {
      resolved.error = msg.error;
    }
    else {
      resolved.error = []
    }

    if (msg.opMode !== undefined) {
      resolved.opMode = msg.opMode;
    }
    else {
      resolved.opMode = []
    }

    if (msg.complement !== undefined) {
      resolved.complement = msg.complement;
    }
    else {
      resolved.complement = []
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

    if (msg.dc_v !== undefined) {
      resolved.dc_v = msg.dc_v;
    }
    else {
      resolved.dc_v = []
    }

    return resolved;
    }
};

module.exports = driverState;
