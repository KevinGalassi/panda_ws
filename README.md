# panda_ws

COMPILAZIONE WORKSPACE ROBOT: da terminale NON ROOT
COMPILAZIONE WORKSPACE PERSONALE: da terminale NON ROOT con attivo: ->source devel/setup.bash del workspace del robot (non il personale)




Franka Emika forum: ttps://www.franka-community.de/faq

ERROR RECOVERY: rostopic pub -1 /franka_control/error_recovery/goal franka_control/ErrorRecoveryActionGoal "{}"

To compile: catkin_make -DCMAKE_BUILD_TYPE=Release -DFranka_DIR:PATH=/home/panda/libfranka/build

ROBOT_IP 172.16.0.2





