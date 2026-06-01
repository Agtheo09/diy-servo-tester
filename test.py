import motor
from hub import port, motion_sensor
import time
import runloop

MOTOR_MAX = 1050.0  # deg/sec
leftMotorPort = port.A
rightMotorPort = port.E

motion_sensor.set_yaw_face(motion_sensor.TOP)

angle_offset = 0


def clip(value, min_val, max_val):
    if value < min_val:
        return min_val

    if value > max_val:
        return max_val

    return value


def speed_profile(x, top_speed, length, accel_percents, start_speed=5):
    # Cliping
    top_speed = clip(top_speed, 0, 100)
    x = clip(x, 0, length)

    accel_length = accel_percents[0] * length
    decel_length = accel_percents[1] * length
    const_length = length - accel_length - decel_length

    if x < accel_length:
        # Acceleration phase: from start_speed to top_speed
        a = (top_speed - start_speed) / accel_length
        return start_speed + a * x
    elif x <= accel_length + const_length:
        # Constant speed phase
        return top_speed
    else:
        # Deceleration phase: from top_speed to start_speed
        x_decel = x - (accel_length + const_length)
        d = (top_speed - start_speed) / decel_length
        return top_speed - d * x_decel


def gyro_straight(top_speed, target, accel_percents, Kp, Kd, condition_type, condition):
    global angle_offset

    # Reset Motor Positions to 0
    motor.reset_relative_position(leftMotorPort, 0)
    motor.reset_relative_position(rightMotorPort, 0)

    target *= 10

    # Initialize Timer
    init_time = time.ticks_ms() / 1000

    # Initialize some Variables
    error = 0
    prev_error = 0

    prev_gyro_value = motion_sensor.tilt_angles()[0]
    while True:
        time_elapsed = time.ticks_ms() / 1000 - init_time
        distance_travelled = (
            abs(motor.relative_position(leftMotorPort))
            + abs(motor.relative_position(rightMotorPort))
        ) / 2

        # Calculate Speed
        x = time_elapsed if condition_type == "seconds" else distance_travelled

        speed = speed_profile(x, abs(top_speed), condition, accel_percents)

        # Get robot's Heading
        raw_gyro_value = motion_sensor.tilt_angles()[0]

        # Calculate Continious Value
        if abs(raw_gyro_value - prev_gyro_value) >= 1800:
            angle_offset += -3600 if raw_gyro_value > prev_gyro_value else 3600

        gyro_value = raw_gyro_value + angle_offset
        prev_gyro_value = raw_gyro_value

        # Calculate Error
        error = target * 10 - gyro_value

        # Calculate PD Controller Parts
        P = error * Kp
        D = (error - prev_error) * Kd

        # Calculate PD Controller Output
        output = (P + D) / 10

        if top_speed > 0:
            speedSign = 1
        else:
            speedSign = -1

        speed *= speedSign

        # Steer the robot (Assign speeds based on the Controller Output)
        motor.run(leftMotorPort, int((speed + output) / 100.0 * MOTOR_MAX))
        motor.run(rightMotorPort, -1 * int((speed - output) / 100.0 * MOTOR_MAX))

        prev_error = error

        # Conditions (Breaking the control loop after condition)
        if condition_type == "seconds":
            if time_elapsed >= condition:
                break
        elif condition_type == "degrees":
            if distance_travelled >= condition:
                break
    # Stop Motors
    motor.stop(leftMotorPort, stop=motor.BRAKE)
    motor.stop(rightMotorPort, stop=motor.BRAKE)


def gyro_turn(target_angle, Kp, Ki, Kd, duration):
    global angle_offset
    # Initialize Timer
    init_time = time.ticks_ms() / 1000

    target_angle *= 10

    # Initialize some Variables
    error = 0
    prev_error = 0

    P = 0
    I = 0
    D = 0

    prev_gyro_value = motion_sensor.tilt_angles()[0]
    while True:
        time_elapsed = time.ticks_ms() / 1000 - init_time

        # Get robot's Heading
        raw_gyro_value = motion_sensor.tilt_angles()[0]

        # Calculate Continious Value
        if abs(raw_gyro_value - prev_gyro_value) >= 1800:
            angle_offset += -3600 if raw_gyro_value > prev_gyro_value else 3600

        gyro_value = raw_gyro_value + angle_offset

        prev_gyro_value = raw_gyro_value

        # Calculate Error
        error = target_angle - gyro_value

        # Calculate PD Controller Parts
        P = error * Kp
        I += error * Ki
        D = (error - prev_error) * Kd

        # Calculate PD Controller Output
        output = (P + D) / 10

        # Steer the robot (Assign speeds based on the Controller Output)
        motor.run(leftMotorPort, int((output) / 100.0 * MOTOR_MAX))
        motor.run(rightMotorPort, -1 * int((-output) / 100.0 * MOTOR_MAX))

        prev_error = error

        # Condition (Breaking the control loop after duration)
        if time_elapsed >= duration:
            break
    # Stop Motors
    motor.stop(leftMotorPort, stop=motor.BRAKE)
    motor.stop(rightMotorPort, stop=motor.BRAKE)


# gyro_straight(60, 0, (0.2, 0.2), 1.5, 1.8, "degrees", 2000) # top_speed, target_heading, (accel_perc, decel_perc), Kp, Kd, condition_type, condition
# gyro_turn(90, 1.5, 2, 2) # target_angle, Kp, Kd, duration

# Square
# for _ in range(1):
#     for i in range(12):
#         # gyro_straight(80, 90*i, (0, 0), 1.5, 0, "degrees", 800)
#         gyro_turn(90*(i+1), 1.8, 10000000000, 0, 3)
#         time.sleep(2)


gyro_straight(80, 0, (0.1, 0.1), 1.5, 2, "seconds", 10)
# gyro_turn(180, 1.5, 10, 0, 1.2)
