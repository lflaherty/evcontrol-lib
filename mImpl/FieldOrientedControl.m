classdef FieldOrientedControl < matlab.System & matlab.system.mixin.Propagates
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
    end

    properties(DiscreteState)

    end

    % Pre-computed constants
    properties(Access = private)
        CurrentRef;
        CurrentController;
        PWM;
    end
    
    methods(Access = public)
        function init(obj)
            obj.CurrentRef = PMSMCurrentRef;
            obj.CurrentRef.Vnom = obj.Vnom;
            obj.CurrentRef.Pmax = obj.Pmax;
            obj.CurrentRef.Tmax = obj.Tmax;
            obj.CurrentRef.Imax = obj.Imax;
            obj.CurrentRef.wBase = obj.wBase;
            obj.CurrentRef.p = obj.p;
            obj.CurrentRef.psim = obj.psim;
            obj.CurrentRef.Ld = obj.Ld;
            
            obj.CurrentController = PMSMCurrentController;
            obj.CurrentController.T = obj.T;
            obj.CurrentController.Kp_id = obj.Kp_id;
            obj.CurrentController.Ki_id = obj.Ki_id;
            obj.CurrentController.AntiWindUpD = obj.AntiWindUpD;
            obj.CurrentController.Kp_iq = obj.Kp_iq;
            obj.CurrentController.Ki_iq = obj.Ki_iq;
            obj.CurrentController.AntiWindUpQ = obj.AntiWindUpQ;
            obj.CurrentController.init();
            
            obj.PWM = SPWM;
        end
    end

    methods(Access = protected)
        function setupImpl(obj)
            obj.init();
        end

        function [Dabc, Vdq, idqRef, idq, TqRefSat, TqLim, TqEst, rpm_base] = stepImpl(obj, TqRef, we, iabc, theta_e, Vdc)
            wMech = we/obj.p;
            
            % Estimate some things:
            idq = parkTransform(iabc, theta_e);
            TqEst = torqueEst(obj.p, obj.psim, idq, [obj.Ld; obj.Lq]);
            [~, rpm_base] = obj.calcBaseSpeed(idq, Vdc);
            
            % Perform control:
            [idqRef, TqRefSat, TqLim] = obj.CurrentRef.stepImpl(TqRef, wMech, Vdc);
            [Vdq, ~] = obj.CurrentController.stepImpl(idqRef, iabc, theta_e, we, Vdc);
            Dabc = obj.PWM.stepImpl(Vdq, theta_e, Vdc);
        end

        function resetImpl(obj)
            % Initialize / reset discrete-state properties
            obj.CurrentController.resetImpl();
            obj.PWM.resetImpl();
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
        function [sz1, sz2, sz3, sz4, sz5, sz6, sz7, sz8] = getOutputSizeImpl(obj)            
            sz1 = [3,1];
            sz2 = [2,1];
            sz3 = [2,1];
            sz4 = [2,1];
            sz5 = [1,1];
            sz6 = [1,1];
            sz7 = [1,1];
            sz8 = [1,1];
        end
        
        function [fz1, fz2, fz3, fz4, fz5, fz6, fz7, fz8] = isOutputFixedSizeImpl(~)
            fz1 = true;
            fz2 = true;
            fz3 = true;
            fz4 = true;
            fz5 = true;
            fz6 = true;
            fz7 = true;
            fz8 = true;
        end
        
        function [dt1, dt2, dt3, dt4, dt5, dt6, dt7, dt8] = getOutputDataTypeImpl(obj)
            dt1 = 'double';
            dt2 = 'double';
            dt3 = 'double';
            dt4 = 'double';
            dt5 = 'double';
            dt6 = 'double';
            dt7 = 'double';
            dt8 = 'double';
        end
    
        function [cp1, cp2, cp3, cp4, cp5, cp6, cp7, cp8] = isOutputComplexImpl(obj)
            cp1 = false;
            cp2 = false;
            cp3 = false;
            cp4 = false;
            cp5 = false;
            cp6 = false;
            cp7 = false;
            cp8 = false;
        end
    end
end
