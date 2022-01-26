function w = hallSensorSpeedCalc(hallA, hallB, hallC, CLOCK_COUNTS, CLOCK_FREQ)
%HALLSENSORSPEEDCALC Returns w as a speed est from hall effect inputs [rad/s]
    
    persistent init
    persistent counter
    persistent hallCounts
    persistent prevHalls
    persistent result
    if isempty(init)
        init = 1;
        counter = 0;
        hallCounts = 0;
        prevHalls = [false, false, false];
        result = 0;
    end

    % Hall effect counters
    if prevHalls(1) == false && hallA == true
        hallCounts = hallCounts + 1;
    end
    if prevHalls(2) == false && hallB == true
        hallCounts = hallCounts + 1;
    end
    if prevHalls(3) == false && hallC == true
        hallCounts = hallCounts + 1;
    end
    
    prevHalls = [hallA, hallB, hallC];

    % Final counter for calculation
    counter = counter+1;
    if counter == CLOCK_COUNTS
        clock_period = 1/CLOCK_FREQ;
        timing_period = clock_period * CLOCK_COUNTS; % [s]

        % Saw hallCounts number of 0->1 hall transitions over timing_period
        % 1 0->1 transition on a hall effect phase => revolution
        % 3 0->1 transitions across hallA-C => 1 rev.
        revPerSecond = hallCounts / timing_period;
        revPerSecond = revPerSecond / 3; % Across 3 hall signals
        revPerSecond = revPerSecond / 10; % pole pairs
        rpm = revPerSecond * 60;

        % Update result
        result = rpm * pi/30;

        % Reset counters
        hallCounts = 0;
        counter = 0;
    end

    w = result;

end

