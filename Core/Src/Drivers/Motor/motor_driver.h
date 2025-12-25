typedef struct {
	TIM_HandleTypeDef* htim;
} motor_t;

void motor_init(motor_t* motor);
void motor_move(float pitch);
