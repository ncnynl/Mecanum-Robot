//
// Demo.ino
//

boolean active = false;
unsigned long starttime;

// =============================================================================
// Helper structures

void wheelVelocities(float *velocities, float vx, float vy, float omega)
{
    float s1 = vy - vx;
    float s2 = vy + vx;

    velocities[0] = s1 + omega;
    velocities[1] = s2 - omega;
    velocities[2] = s1 - omega;
    velocities[3] = s2 + omega;
}


// =============================================================================
// Define the demos

// speed multiplier: 5000, timeStretch 0.093
CurveResult circle_translation_only(float t)
{
    float timeStretch = 0.093;

    CurveResult result;
    result.finished = false;
    result.omega = 0;
    result.vx = sin(t * timeStretch);
    result.vy = cos(t * timeStretch);

    // stop after a quarter circle
    if (result.vy < 0)
    {
        result.finished = true;
    }

    return result;
}

CurveResult circle_with_rotation(float t)
{
    float timeStretch = 0.1;

    CurveResult result;
    result.finished = false;
    result.omega = -0.3;
    result.vx = 0;
    result.vy = 1;

    return result;
}


// =============================================================================
// Demo execution code

void demo_start()
{
    starttime = millis();
    active = true;
}

void demo_update()
{
    if (!active) return;

    float t = (millis() - starttime) / 1000.0;

    // CurveResult result = circle_translation_only(t);
    CurveResult result = circle_with_rotation(t);

    // stop when demo is finished
    if (result.finished) demo_stop();

    float velocities[4] = {0, 0, 0, 0};
    wheelVelocities(velocities, result.vx, result.vy, result.omega);

    for (int i = 0; i < 4; i++)
    {
        Serial.print((int)(velocities[i] * 5000));
        Serial.print("\t");
        robot_setMotorSpeed(i, (int)(velocities[i] * 5000));
    }
    Serial.println();
}

void demo_stop()
{
    active = false;
    for (int i = 0; i < 4; i++)
    {
        robot_quickStop(i);
    }
}
