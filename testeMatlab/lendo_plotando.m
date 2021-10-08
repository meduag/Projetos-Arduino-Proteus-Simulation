arduinoObj = serialport("COM6",9600);
configureTerminator(arduinoObj,"CR/LF");
flush(arduinoObj);
arduinoObj.UserData = struct("Data",[],"Count",1);

data = zeros(1, 1000);

for R =1:1000
    data(1,R) = readline(arduinoObj);
    arduinoObj.UserData.Data(end+1) = str2double(data);
    arduinoObj.UserData.Count = arduinoObj.UserData.Count + 1;
end

if arduinoObj.UserData.Count > 1001
    configureCallback(arduinoObj, "off");
    plot(arduinoObj.UserData.Data(2:end));
end