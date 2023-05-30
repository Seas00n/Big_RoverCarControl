# This Python file uses the following encoding: utf-8
"""autogenerated by genpy from car_control/corner.msg. Do not edit."""
import codecs
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct


class corner(genpy.Message):
  _md5sum = "f28300dc96da5592dc5c99f06bac2854"
  _type = "car_control/corner"
  _has_header = False  # flag to mark the presence of a Header object
  _full_text = """float64 wheel_pos_actual
float64 wheel_pos_desired
float64 wheel_vel_actual
float64 wheel_vel_desired
float64 steer_pos_actual
float64 steer_pos_desired
float64 steer_vel_actual
float64 steer_vel_desired"""
  __slots__ = ['wheel_pos_actual','wheel_pos_desired','wheel_vel_actual','wheel_vel_desired','steer_pos_actual','steer_pos_desired','steer_vel_actual','steer_vel_desired']
  _slot_types = ['float64','float64','float64','float64','float64','float64','float64','float64']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       wheel_pos_actual,wheel_pos_desired,wheel_vel_actual,wheel_vel_desired,steer_pos_actual,steer_pos_desired,steer_vel_actual,steer_vel_desired

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(corner, self).__init__(*args, **kwds)
      # message fields cannot be None, assign default values for those that are
      if self.wheel_pos_actual is None:
        self.wheel_pos_actual = 0.
      if self.wheel_pos_desired is None:
        self.wheel_pos_desired = 0.
      if self.wheel_vel_actual is None:
        self.wheel_vel_actual = 0.
      if self.wheel_vel_desired is None:
        self.wheel_vel_desired = 0.
      if self.steer_pos_actual is None:
        self.steer_pos_actual = 0.
      if self.steer_pos_desired is None:
        self.steer_pos_desired = 0.
      if self.steer_vel_actual is None:
        self.steer_vel_actual = 0.
      if self.steer_vel_desired is None:
        self.steer_vel_desired = 0.
    else:
      self.wheel_pos_actual = 0.
      self.wheel_pos_desired = 0.
      self.wheel_vel_actual = 0.
      self.wheel_vel_desired = 0.
      self.steer_pos_actual = 0.
      self.steer_pos_desired = 0.
      self.steer_vel_actual = 0.
      self.steer_vel_desired = 0.

  def _get_types(self):
    """
    internal API method
    """
    return self._slot_types

  def serialize(self, buff):
    """
    serialize message into buffer
    :param buff: buffer, ``StringIO``
    """
    try:
      _x = self
      buff.write(_get_struct_8d().pack(_x.wheel_pos_actual, _x.wheel_pos_desired, _x.wheel_vel_actual, _x.wheel_vel_desired, _x.steer_pos_actual, _x.steer_pos_desired, _x.steer_vel_actual, _x.steer_vel_desired))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(locals().get('_x', self)))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(locals().get('_x', self)))))

  def deserialize(self, str):
    """
    unpack serialized message in str into this message instance
    :param str: byte array of serialized message, ``str``
    """
    if python3:
      codecs.lookup_error("rosmsg").msg_type = self._type
    try:
      end = 0
      _x = self
      start = end
      end += 64
      (_x.wheel_pos_actual, _x.wheel_pos_desired, _x.wheel_vel_actual, _x.wheel_vel_desired, _x.steer_pos_actual, _x.steer_pos_desired, _x.steer_vel_actual, _x.steer_vel_desired,) = _get_struct_8d().unpack(str[start:end])
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e)  # most likely buffer underfill


  def serialize_numpy(self, buff, numpy):
    """
    serialize message with numpy array types into buffer
    :param buff: buffer, ``StringIO``
    :param numpy: numpy python module
    """
    try:
      _x = self
      buff.write(_get_struct_8d().pack(_x.wheel_pos_actual, _x.wheel_pos_desired, _x.wheel_vel_actual, _x.wheel_vel_desired, _x.steer_pos_actual, _x.steer_pos_desired, _x.steer_vel_actual, _x.steer_vel_desired))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(locals().get('_x', self)))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(locals().get('_x', self)))))

  def deserialize_numpy(self, str, numpy):
    """
    unpack serialized message in str into this message instance using numpy for array types
    :param str: byte array of serialized message, ``str``
    :param numpy: numpy python module
    """
    if python3:
      codecs.lookup_error("rosmsg").msg_type = self._type
    try:
      end = 0
      _x = self
      start = end
      end += 64
      (_x.wheel_pos_actual, _x.wheel_pos_desired, _x.wheel_vel_actual, _x.wheel_vel_desired, _x.steer_pos_actual, _x.steer_pos_desired, _x.steer_vel_actual, _x.steer_vel_desired,) = _get_struct_8d().unpack(str[start:end])
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e)  # most likely buffer underfill

_struct_I = genpy.struct_I
def _get_struct_I():
    global _struct_I
    return _struct_I
_struct_8d = None
def _get_struct_8d():
    global _struct_8d
    if _struct_8d is None:
        _struct_8d = struct.Struct("<8d")
    return _struct_8d
