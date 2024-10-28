classdef FieldOrientedSpeedControl < matlab.System & matlab.system.mixin.Propagates
    % Implements Sinusoidal PWM

    % Public, tunable properties
    properties
        T = 0.001; % Sample time
        
        Vnom = 0;
        Pmax = 0;
        Tmax = 0;
        Imax = 0;
        p = 1;
        psim = 0;
        
        Rs = 0;
        Ld = 0;
        Lq = 0;
        
        wBase = 0; % Base speed [rad/s]
        
        Kp_id = 1;
        Ki_id = 1;
        AntiWindUpD = 1;
        Kp_iq = 1;
        Ki_iq = 1;
        AntiWindUpQ = 1;
        
        Kp_speed = 1; % Speed PI P gain
        Ki_speed = 0; % Speed PI I gain
        AntiWindUpSpeed = 1;
    end

    properties(DiscreteState)

    end

    % Pre-computed constants
    properties(Access = private)
        FOC;
        PI_speed;
    end

    methods(Access = protected)
        function setupImpl(obj)
            obj.FOC = FieldOrientedControl;
            obj.FOC.T = obj.T;
            
            obj.FOC.Vnom = obj.Vnom;
            obj.FOC.Pmax = obj.Pmax;
            obj.FOC.Tmax = obj.Tmax;
            obj.FOC.Imax = obj.Imax;
            obj.FOC.p = obj.p;
            obj.FOC.psim = obj.psim;
            
            obj.FOC.Rs = obj.Rs;
            obj.FOC.Ld = obj.Ld;
            obj.FOC.Lq = obj.Lq;
            
            obj.FOC.wBase = obj.wBase;
            
            obj.FOC.Kp_id = obj.Kp_id;
            obj.FOC.Ki_id = obj.Ki_id;
            obj.FOC.AntiWindUpD = obj.AntiWindUpD;
            obj.FOC.Kp_iq = obj.Kp_iq;
            obj.FOC.Ki_iq = obj.Ki_iq;
            obj.FOC.AntiWindUpQ = obj.AntiWindUpQ;
            
            obj.FOC.init();
            
            obj.PI_speed = DiscretePID;
            obj.PI_speed.T = obj.T;
            obj.PI_speed.Kp = obj.Kp_speed;
            obj.PI_speed.Ki = obj.Ki_speed;
            obj.PI_speed.Kd = 0;
            %obj.PI_speed.upperLimitInt = obj.AntiWindUpSpeed; % TODO anti windup doesn't work properly
            %obj.PI_speed.lowerLimitInt = -obj.AntiWindUpSpeed;
            %obj.PI_speed.setupImpl();     % TODO need to figure out how to call these
        end

        function [Dabc, Vdq, idqRef, TqRefSat, TqLim, TqEst, rpm_base] = stepImpl(obj, RpmRef, RpmMeas, TqEnable, we, iabc, theta_e, Vdc)
            if TqEnable > 0
                tqRef = obj.PI_speed.stepImpl(RpmRef, RpmMeas);
            else
                tqRef = 0.0;
            end
            
            [Dabc, Vdq, idqRef, TqRefSat, TqLim, TqEst, rpm_base] = obj.FOC.stepImpl(tqRef, we, iabc, theta_e, Vdc);
        end

        function resetImpl(obj)
            % Initialize / reset discrete-state properties
            obj.FOC.resetImpl();
        end
        
        function [w_base, rpm_base] = calcBaseSpeed(obj, idq, Vdc)
            i_max = sqrt(3)*obj.Pmax/Vdc;
            v_max = Vdc/sqrt(3) - obj.Rs*i_max;
            
            id = idq(1);
            iq = idq(2);

            w_base = (1/obj.p)* v_max/sqrt((obj.Lq*iq)^2+(obj.Ld*id+obj.psim)^2);
            rpm_base = w_base * 30/pi;
        end
    
        %% Output sizing
        function [sz1, sz2, sz3, sz4, sz5, sz6, sz7] = getOutputSizeImpl(obj)            
            sz1 = [3,1];
            sz2 = [2,1];
            sz3 = [2,1];
            sz4 = [1,1];
            sz5 = [1,1];
            sz6 = [1,1];
            sz7 = [1,1];
        end
        
        function [fz1, fz2, fz3, fz4, fz5, fz6, fz7] = isOutputFixedSizeImpl(~)
            fz1 = true;
            fz2 = true;
            fz3 = true;
            fz4 = true;
            fz5 = true;
            fz6 = true;
            fz7 = true;
        end
        
        function [dt1, dt2, dt3, dt4, dt5, dt6, dt7] = getOutputDataTypeImpl(obj)
            dt1 = 'double';
            dt2 = 'double';
            dt3 = 'double';
            dt4 = 'double';
            dt5 = 'double';
            dt6 = 'double';
            dt7 = 'double';
        end
    
        function [cp1, cp2, cp3, cp4, cp5, cp6, cp7] = isOutputComplexImpl(obj)
            cp1 = false;
            cp2 = false;
            cp3 = false;
            cp4 = false;
            cp5 = false;
            cp6 = false;
            cp7 = false;
        end
    end
end
