classdef PMSMCurrentController < matlab.System & matlab.system.mixin.Propagates
    % Implements field weakening of idqRef input

    % Public, tunable properties
    properties
        Kp_id = 1;
        Ki_id = 1;
        AntiWindUpD = 1;
        Kp_iq = 1;
        Ki_iq = 1;
        AntiWindUpQ = 1;
        T = 0.001;
    end

    properties(DiscreteState)

    end

    % Pre-computed constants
    properties(Access = private)
        PI_id;
        PI_iq;
    end
    
    methods(Access = public)
        function init(obj)
            obj.PI_id = DiscretePID;
            obj.PI_id.T = obj.T;
            obj.PI_id.Kp = obj.Kp_id;
            obj.PI_id.Ki = obj.Ki_id;
            obj.PI_id.Kd = 0;
            %obj.PI_id.upperLimitInt = obj.AntiWindUpD; % TODO anti windup doesn't work properly
            %obj.PI_id.lowerLimitInt = -obj.AntiWindUpD;
            %obj.PI_id.setupImpl();     % TODO need to figure out how to call these
            
            obj.PI_iq = DiscretePID;
            obj.PI_iq.T = obj.T;
            obj.PI_iq.Kp = obj.Kp_iq;
            obj.PI_iq.Ki = obj.Ki_iq;
            obj.PI_iq.Kd = 0;
            %obj.PI_iq.upperLimitInt = obj.AntiWindUpQ;
            %obj.PI_iq.lowerLimitInt = -obj.AntiWindUpQ;
            %obj.PI_iq.setupImpl();
        end
    end

    methods(Access = protected)
        function setupImpl(obj)
            obj.init();
        end

        function [vdq, idq] = stepImpl(obj, idqRef, iabc, theta, we, Vdc)
            % Apply a Park transform to obtain idq
            idq = parkTransform(iabc, theta);
            id = idq(1);
            iq = idq(2);
            
            % Valculate Vphmax
            Vphmax = Vdc/sqrt(3);
            
            % D axis PI controller
            %obj.PI_id.upperLimit = Vphmax;
            %obj.PI_id.lowerLimit = -Vphmax;
            vd = obj.PI_id.stepImpl(idqRef(1), id);
            
            % Q axis PI controller
            %obj.PI_iq.upperLimit = Vphmax;
            %obj.PI_iq.lowerLimit = -Vphmax;
            vq = obj.PI_iq.stepImpl(idqRef(2), iq);
            
            % TODO feedforward control
            
            % d-q voltage limiter - Q axis prioritization
            % vphmax^2 = vd^2 + vq^2
            vq_sat = sat(vq, -Vphmax, Vphmax);
            vd_lim = sqrt(Vphmax^2 - vq_sat^2);
            vd_sat = sat(vd, -vd_lim, vd_lim);
            
            vdq = [vd_sat; vq_sat];
        end
        
        function resetImpl(obj)
            % Initialize / reset discrete-state properties
            obj.PI_id.resetImpl();
            obj.PI_iq.resetImpl();
        end
    
        %% Output sizing
        function [sz1, sz2] = getOutputSizeImpl(~)
            sz1 = [2,1];
            sz2 = [2,1];
        end
        
        function [fz1, fz2] = isOutputFixedSizeImpl(~)
          fz1 = true;
          fz2 = true;
        end
        
        function [dt1, dt2] = getOutputDataTypeImpl(~)
            dt1 = 'double';
            dt2 = 'double';
        end
    
        function [cp1, cp2] = isOutputComplexImpl(~)
            cp1 = false;
            cp2 = false;
        end
    end
end
