function [out] = sat(value,peak)
%SAT Summary of this function goes here
%   Detailed explanation goes here

    if value > peak
        out = peak;
    elseif value < -peak
        out = -peak;
    else
        out = value;
    end

end

