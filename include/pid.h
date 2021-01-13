struct PID
{
	uint8_t kp;
	uint8_t ki;
	uint8_t kd;

	int16_t err;
	int16_t err_;
	int16_t err__;

	int16_t err_integ;
	int16_t err_diff;

	uint8_t dead_band;
	int16_t cv;
	int16_t sp;
	int16_t pv;
};


	


