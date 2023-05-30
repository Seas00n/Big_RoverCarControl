# Big_RoverCarControl
使用手柄控制无人车完成运动，同时配置一个基于turtlesim的仿真环境
## Installation
```
git clone https://github.com/Seas00n/Big_RoverCarControl.git
cd /path/to/Big_RoverCarControl
catkin_make -j8
```
## Dependencies
手柄配置 参考
```
https://www.yahboom.com/study/USB-PS2
```
```
sudo apt install ros-<distro>-joy 
ros-<distro>-joystick-drivers

sudo apt install jstets-gtk
```
## 按键预览
```
sudo jstest-gtk
```
根据按键显示配置``/src/car_control/scripts/joy_listener.py``
为了防止指令突变，添加了二阶按键阻尼，运行
```
cd /src/car_control/scripts
python tune_dynamic.py
```
查看不同f,z,r对应的指令跟踪效果

## 软件架构
### 手柄：

joy_listener节点监听joy节点，根据按键和操纵杆输出将经过二阶系统平滑后的速度下发到rover_state节点

### 主函数：
订阅rover_state节点，触发对应控制方法的回调函数

总共三个线程：接受线程接收电机返回数据，发送线程发送期望速度位置指令

steering_Motor为舵机的硬件接口，wheel_Motor为车轮的硬件接口，硬件接口在编写控制软件时不需要设置，只会在接收，发送线程中进行数据的交换。

actuators为舵轮整体的软件抽象层，包括舵轮朝向和车轮速度等

rover_temp为全车状态的软件抽象层，包括期望位置速度，运行状态

控制器回调函数需要在每次接受到新的rover_state后设置rover_temp的期望速度，并且根据逆运动学解算出当前控制方法整车速度对应的各个车轮的期望速度和期望朝向，根据此设置actuators. 

### Turtlesim仿真系统

为了在软件编写时检查舵轮朝向和速度是否正确，提供了一个仿真系统，仿真系统用turtle显示每个轮子的运动

rover_temp的期望速度会直接发送到turtle1中，并订阅turtle1的Pose节点信息，传入里程计函数中作为全车的实际位置和实际朝向





