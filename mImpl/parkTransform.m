function [idq] = parkTransform(iabc,theta)
    phase = 2*pi/3;
    half = 1/2;
    M = [sin(theta), sin(theta - phase), sin(theta + phase); ...
         cos(theta), cos(theta - phase), cos(theta + phase); ...
         half,       half,               half];
    idq0 = (2/3)*M*iabc;
    id = idq0(1);
    iq = idq0(2);
    idq = [id; iq];
end

