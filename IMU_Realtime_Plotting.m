% ======================= MATLAB REALTIME IMU PLOT =======================

clear; clc;

port = "COM21";                     % Change it accordingly
baud = 115200;

s = serialport(port, baud);
configureTerminator(s, "LF");
pause(2);
flush(s);

samplesVisible = 250;               % samples visible on screen

accelerometerReadings  = zeros(samplesVisible,3);
gyroscopeReadings = zeros(samplesVisible,3);

Fs = 100;                           % Arduino output rate (Hz)

plotTime = (-(samplesVisible-1): 0) / Fs;

figure;

% ---- Accelerometer plot ----
ax1 = subplot(2,1,1);
hAcc = plot(plotTime, accelerometerReadings);
title("Accelerometer (g)")
legend X Y Z
grid on
ylim([-2 2])

% ---- Gyroscope plot ----
ax2 = subplot(2,1,2);
hGyro = plot(plotTime, gyroscopeReadings);
title("Gyroscope (deg/s)")
legend X Y Z
grid on
ylim([-250 250])

% ---- Main Loop ----

while true
    line = readline(s);
    data = sscanf(line, '%f,%f,%f,%f,%f,%f');

    if numel(data) ~= 6
        continue
    end

    accNew  = data(1:3)';
    gyroNew = data(4:6)';

    % ---- Shift buffers ----
    accelerometerReadings(1:end-1,:) = accelerometerReadings(2:end,:);
    gyroscopeReadings(1:end-1,:)     = gyroscopeReadings(2:end,:);

    accelerometerReadings(end,:) = accNew;
    gyroscopeReadings(end,:)     = gyroNew;

    % ---- Update plots ----
    for i = 1:3
        hAcc(i).YData  = accelerometerReadings(:,i);
        hGyro(i).YData = gyroscopeReadings(:,i);
    end

    drawnow limitrate nocallbacks
end
