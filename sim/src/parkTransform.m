function [idq] = parkTransform(iabc,theta)
%     phase = 2*pi/3;
%     half = 1/2;
%     M = [sin(theta), sin(theta - phase), sin(theta + phase); ...
%          cos(theta), cos(theta - phase), cos(theta + phase); ...
%          half,       half,               half];
%     idq0 = (2/3)*M*iabc;
%     id = idq0(1);
%     iq = idq0(2);
%     idq = [id; iq];

    ia = iabc(1);
    ib = iabc(2);
    ic = iabc(3);

    phase = 2*pi/3;
    id = sin(theta)*ia + sin(theta-phase)*ib + sin(theta+phase)*ic;
    id = (2/3)*id;
    
    iq = cos(theta)*ia + cos(theta-phase)*ib + cos(theta+phase)*ic;
    iq = (2/3)*iq;
    
    idq = [id; iq];
end

