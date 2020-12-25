const std::vector<double> arm_ready_state       = {0, -0.785, 0, -2.356, 0, 1.571, 0.785};
const std::vector<double> hand_ready_state      = {0.04, 0.04};
const std::vector<double> hand_open_position    = {0.037, 0.037};
const std::vector<double> hand_closed_position  = {0.0, 0.0};
const std::vector<double> hand_grasp_position  = {0.004, 0.004};

// Denavit-Hartenberg Parameters of Panda
const float DH_param_a[8]       = {0, 0, 0, 0.0825, -0.0825, 0, 0.088, 0 };
const float DH_param_d[8]       = {0.333, 0, 0.316, 0, 0.384, 0,  0, 0.107};
const float DH_param_alpha[8]   = {0, -M_PI/2, M_PI/2, M_PI/2, -M_PI/2, M_PI/2, M_PI/2, 0};

// Panda limits
const double q_ul[7]        = {0, 0, 0, 0, 0, 0, 0};
const double q_ll[7]        = {0, 0, 0, 0, 0, 0, 0};
const double qd_max[7]	    = {2.1750, 2.1750, 2.1750, 2.1750, 2.6100, 2.6100, 2.6100};
const double qdd_max[7]	    = {15, 7.5, 10, 12.5, 15, 20, 20};
const double qddd_max[7]    = {7500, 3750, 5000, 6250, 7500, 10000, 10000};

const double gripper_offset[3] = {0, 0, 0.1};