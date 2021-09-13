classdef DiscretePID < matlab.System & matlab.system.mixin.Propagates
    % Implements field weakening of idqRef input

    % Public, tunable properties
    properties
        % Parameters
        T = 0.001       % Sample time
        upperLimit = inf
        lowerLimit = -inf
        upperLimitInt = inf
        lowerLimitInt = -inf
        Kp = 1
        Ki = 1
        Kd = 1
        tau = 1   % derivative filter time constant
    end

    properties(DiscreteState)

    end

    % Pre-computed constants
    properties(Access = private)
        % Variables
        integrator = 0
        differentiator = 0
        prevError = 0
        prevMeas = 0
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
            if obj.upperLimitInt > p
                integratorUpper = obj.upperLimitInt - p;
            else
                integratorUpper = 0;
            end
            
            if obj.lowerLimitInt < p
                integratorLower = obj.lowerLimitInt - p;
            else
                integratorLower = 0;
            end
            
            % Integrator limit
            if obj.integrator > integratorUpper
                obj.integrator = integratorUpper;
            elseif obj.integrator < integratorLower
                obj.integrator = integratorLower;
            end
            
            % Derivative
            obj.differentiator = (2 * obj.Kd * (meas - obj.prevMeas) ...
                                  + (2 * obj.tau - obj.T) * obj.differentiator) ...
                                  / (2 * obj.tau + obj.T);
            
            % output
            y = p + obj.integrator + obj.differentiator;
            
            % Saturation
            y = sat(y, obj.lowerLimit, obj.upperLimit);
            
            obj.prevError = error;
            obj.prevMeas = meas;
        end

        function resetImpl(obj)
            % Initialize / reset discrete-state properties
            obj.integrator = 0;
            obj.differentiator = 0;
            obj.prevError = 0;
            obj.prevMeas = 0;
        end
    
        %% Output sizing
        function [sz1] = getOutputSizeImpl(~)            
            sz1 = 1;
        end
        
        function [fz1] = isOutputFixedSizeImpl(~)
          %Both outputs are always variable-sized
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
