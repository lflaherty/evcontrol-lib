classdef PMSMCurrentRef < matlab.System & matlab.system.mixin.Propagates
    % Implements field weakening of idqRef input

    % Public, tunable properties
    properties
        Vnom = 0;
        Pmax = 0;
        Tmax = 0;
        p = 1;
        psim = 0;
    end

    properties(DiscreteState)

    end

    % Pre-computed constants
    properties(Access = private)

    end

    methods(Access = protected)
        function setupImpl(obj)
            % Perform one-time calculations, such as computing constants
        end

        function [idqRef, TqRefSat, TqLim] = stepImpl(obj, TqRef, wMech, Vdc)
            % Calculate torque limits
            TqLim = (Vdc/obj.Vnom) * min(obj.Tmax, obj.Pmax / sat(abs(wMech), obj.Pmax/obj.Tmax, inf));
            TqRefSat = sat(TqRef, -TqLim, TqLim);
            
            id = 0;
            iq = 2 * TqRefSat / (3 * obj.p * obj.psim);
            idqRef = [id; iq];
            
            % TODO field weakening
        end
        
        function resetImpl(obj)
            % Initialize / reset discrete-state properties
        end
    
        %% Output sizing
        function [sz1, sz2, sz3] = getOutputSizeImpl(~)            
            sz1 = [2,1];
            sz2 = 1;
            sz3 = 1;
        end
        
        function [fz1, fz2, fz3] = isOutputFixedSizeImpl(~)
          %Both outputs are always fixed-sized
          fz1 = true;
          fz2 = true;
          fz3 = true;
        end
        
        function [dt1, dt2, dt3] = getOutputDataTypeImpl(~)
            dt1 = 'double';
            dt2 = 'double';
            dt3 = 'double';
        end
    
        function [cp1, cp2, cp3] = isOutputComplexImpl(~)
            cp1 = false;
            cp2 = false;
            cp3 = false;
        end
    end
end
