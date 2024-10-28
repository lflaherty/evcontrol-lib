function [w_base, rpm_base] = calcBaseSpeed(Vdc,Rs,Pmax,Ldq,idq,psim,p)
%calcBaseSpeed Calculates motor base speed
i_max = sqrt(3)*Pmax/Vdc;
v_max = Vdc/sqrt(3) - Rs*i_max;

Ld = Ldq(1);
Lq = Ldq(2);
id = idq(1);
iq = idq(2);

w_base = (1/p)* v_max/sqrt((Lq*iq)^2+(Ld*id+psim)^2);
rpm_base = w_base * 30/pi;

end

