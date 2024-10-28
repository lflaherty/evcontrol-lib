%% Generates the LUT vectors for the PMSM Current Reference Generator
% Inputs: RPM_VECT, TQ_VECT, VDC_VECT
% Outputs: ID_MAP, IQ_MAP

RPM_VECT = 0:500:8000;  % Index 1
TQ_VECT = -505:5:505;   % Index 2
VDC_VECT = 275:25:650;  % Index 3

ID_MAP = zeros(length(RPM_VECT), length(TQ_VECT), length(VDC_VECT));
IQ_MAP = zeros(length(RPM_VECT), length(TQ_VECT), length(VDC_VECT));

for n=1:length(RPM_VECT)
    for m=1:length(TQ_VECT)
        for o=1:length(VDC_VECT)
            rpm = RPM_VECT(n);
            tq = TQ_VECT(m);
            vdc = VDC_VECT(o);
            
            % Find roots of d-q torque equation (polynomial in terms of iq)
            poly = [9*p^2*(Lq-Ld)^2, 0, 0, 6*tq*p*psim, -4*tq^2];
            iq = roots(poly);

            % Solve for id
            id = psim/(2*(Lq-Ld)) - sqrt(psim^2/(4*(Lq-Ld)^2)+iq.^2);
            
            ID_MAP(n, m, o) = id(4);
            IQ_MAP(n, m, o) = iq(4);
        end
    end
end

clear n m o
clear rpm tq vdc
clear poly polyRoots iq id