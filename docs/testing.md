# Smart Room Testing Notes

## Light Sensor Calibration

| Condition | Sensor Reading |
| Dark |19|
| Dim |223|
| Normal Room |403|
| Bright |989|

## LED Calibration

Threshold Used: 260
LED Response: Correct
Unstable Behavior: None

## Full System Testing

| Test | Temperature | Light |Expected Result | Actual Result | Pass/Fail |
| 1 | Cool | Bright | Fan OFF, Light OFF |Fan OFF, Light OFF |P|
| 2 | Cool | Dark | Fan OFF, Light ON |Fan OFF, Light ON |P|
| 3 | Warm | Bright | Fan ON, Light OFF |Fan ON, Light OFF |P|
| 4 | Warm | Dark | Fan ON, Light ON |Fan ON, Light ON |P|




