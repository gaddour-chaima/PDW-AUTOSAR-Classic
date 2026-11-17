clc;
clear;

models = {'PDW_TestBench', 'PDW_TestBench_SIL'};

results = cell(1, 2);
warning = cell(1, 2);
buzzer = cell(1, 2);

for i = 1:2

    mdl = models{i};
    load_system(mdl);

    % Configurer les memes entrees
    simIn = Simulink.SimulationInput(mdl);

    simIn = simIn.setBlockParameter( ...
        [mdl '/Distance_Test'], ...
        'Value', 'single(0.5)');

    simIn = simIn.setBlockParameter( ...
        [mdl '/ReverseGear_Test'], ...
        'Value', 'true');

    % Configuration temporelle identique
    simIn = simIn.setModelParameter( ...
        'StopTime', '2', ...
        'SolverType', 'Fixed-step', ...
        'Solver', 'FixedStepDiscrete', ...
        'FixedStep', '0.02');

    fprintf('Simulation : %s\n', mdl);

    results{i} = sim(simIn);

    warning{i} = results{i}.get('WarningMIL');
    buzzer{i} = results{i}.get('BuzzerMIL');

end

% Comparer les instants d'echantillonnage
sameTimeWarning = isequal( ...
    warning{1}.Time(:), warning{2}.Time(:));

sameTimeBuzzer = isequal( ...
    buzzer{1}.Time(:), buzzer{2}.Time(:));

% Comparer les valeurs echantillon par echantillon
sameWarning = isequal( ...
    double(warning{1}.Data(:)), ...
    double(warning{2}.Data(:)));

sameBuzzer = isequal( ...
    double(buzzer{1}.Data(:)), ...
    double(buzzer{2}.Data(:)));

% Resultats
fprintf('\n===== COMPARAISON MIL / SIL =====\n');

fprintf('Temps Warning identiques : %d\n', sameTimeWarning);
fprintf('Temps Buzzer identiques  : %d\n', sameTimeBuzzer);

fprintf('WarningLevel identique : %d\n', sameWarning);
fprintf('BuzzerOutput identique : %d\n', sameBuzzer);

if sameTimeWarning && sameTimeBuzzer && ...
        sameWarning && sameBuzzer

    disp('PASS : MIL et SIL equivalentes pour ce test.');

else

    disp('FAIL : Difference MIL / SIL detectee.');

end