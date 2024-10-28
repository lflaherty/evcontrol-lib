function [out] = sat(value, lower, upper)
%SAT Summary of this function goes here
%   Detailed explanation goes here

    if value > upper
        out = upper;
    elseif value < lower
        out = lower;
    else
        out = value;
    end

end

