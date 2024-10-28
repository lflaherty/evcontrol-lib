function [Vabc] = inverseParkTransform(Vdq, theta_e)
%     Vdq0 = [Vdq(1); Vdq(2); 0];
%     phase = 2*pi/3;
%     M = [sin(theta_e),         cos(theta_e),         1; ...
%          sin(theta_e - phase), cos(theta_e - phase), 1; ...
%          sin(theta_e + phase), cos(theta_e + phase), 1];
%     Vabc = M*Vdq0;

    vd = Vdq(1);
    vq = Vdq(2);
    
    phase = 2*pi/3;
    va = sin(theta_e)*vd       + cos(theta_e)*vq;
    vb = sin(theta_e-phase)*vd + cos(theta_e-phase)*vq;
    vc = sin(theta_e+phase)*vd + cos(theta_e+phase)*vq;
    
    Vabc = [va; vb; vc];
end

