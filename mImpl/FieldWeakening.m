classdef FieldWeakening < matlab.System & matlab.system.mixin.Propagates
    % Implements field weakening of idqRef input

    % Public, tunable properties
    properties
        wbase = 0;
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

        function [idqRefFW, FWCtrl] = stepImpl(obj, idqRef, w)
            % Determine field weakening amount
            if w <= obj.wbase
                FWCtrl = 1;
            else
                FWCtrl = 1 - (w - obj.wbase)/obj.wbase;
            end
            FWCtrl = sat(FWCtrl, 0, 1);
            
            % Perform polar/cartesian conversions
            id = idqRef(1);
            iq = idqRef(2);
            [r, theta] = cart2pol(id, iq);
            
            % Scale via FWCtrl (0-1)
            theta = pi - theta;
            theta = theta * FWCtrl;
            theta = pi - theta;
            
            [idFW, iqFW] = pol2cart(r, theta);
            idqRefFW = [idFW; iqFW];
        end

        function resetImpl(obj)
            % Initialize / reset discrete-state properties
        end
    
        %% Output sizing
        function [sz1, sz2] = getOutputSizeImpl(obj)            
            sz1 = [2,1];
            sz2 = 1;
        end
        
        function [fz1, fz2] = isOutputFixedSizeImpl(~)
          %Both outputs are always variable-sized
          fz1 = true;
          fz2 = true;
        end
        
        function [dt1, dt2] = getOutputDataTypeImpl(obj)
            dt1 = propagatedInputDataType(obj, 1); % idqRefFW shares type from idqRef input
            dt2 = 'double'; % FWCtrl is always a double
        end
    
        function [cp1, cp2] = isOutputComplexImpl(obj)
            cp1 = false;
            cp2 = false;
        end
    end
end
