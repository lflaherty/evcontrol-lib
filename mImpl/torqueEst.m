function [TqEst] = torqueEst(p, psim, idq, Ldq)

    TqEst = (3*p/2)*(psim*idq(2)+(Ldq(1)-Ldq(2))*idq(1)*idq(2));

end

