classdef DiscretePID < matlab.System & matlab.system.mixin.Propagates
    % Implements field weakening of idqRef input

    % Public, tunable properties
    properties
        % Parameters
        T = 0.001 % Sample time
        upperLimit = inf % Output upper saturation
        lowerLimit = -inf % Output lower saturation
        upperLimitInt = inf
        lowerLimitInt = -inf
        Kaw = 1 % Anti-windup gain, Kaw
        Kp = 1 % Proportional gain, Kp
        Ki = 1 % Integral gain, Ki
        Kd = 1 % Derivative gain, Kd
        N = 1 % Derivative filter coefficient
    end

    properties(DiscreteState)

    end

    % Pre-computed constants
    properties(Access = private)
        % Variables
        integrator = 0
        differentiator = 0
        prevError = 0
    end

    methods(Access = protected)
        function setupImpl(obj)
            % Perform one-time calculations, such as computing constants
        end

        function y = stepImpl(obj, setpoint, meas)
            % Step operation
            
            error = setpoint - meas;
            
            % Proportional
            p = obj.Kp * error;
            
            % Integral
            obj.integrator = obj.integrator + 0.5 * obj.Ki * obj.T * (error + obj.prevError);
            
            % Integral anti windup
            obj.integrator = sat(obj.integrator, obj.lowerLimitInt, obj.upperLimitInt);
            
            % Derivative
            obj.differentiator = obj.Kd * obj.N * (error - obj.prevError) ...
                                 + (1 - obj.N * obj.T) * obj.differentiator;
            
            % output
            y = p + obj.integrator + obj.differentiator;
            
            % Saturation
            y = sat(y, obj.lowerLimit, obj.upperLimit);
            
            obj.prevError = error;
        end

        function resetImpl(obj)
            % Initialize / reset discrete-state properties
            obj.integrator = 0;
            obj.differentiator = 0;
            obj.prevError = 0;
        end
    
        %% Output sizing
        function [sz1] = getOutputSizeImpl(~)            
            sz1 = 1;
        end
        
        function [fz1] = isOutputFixedSizeImpl(~)
          %Both outputs are always fixed-sized
          fz1 = true;
        end
        
        function [dt1] = getOutputDataTypeImpl(obj)
            dt1 = propagatedInputDataType(obj, 1);
        end
    
        function [cp1] = isOutputComplexImpl(~)
            cp1 = false;
        end
    end
end
