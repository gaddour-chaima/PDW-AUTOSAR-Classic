clc;

% Modeles MIL et SIL
models = {'PDW_TestBench', 'PDW_TestBench_SIL'};

% Cas de test :
% Nom, Distance, ReverseGear, WarningLevel attendu
tests = {
    'MIL-SIL-01', 'single(2.0)',   'true',  0;
    'MIL-SIL-02', 'single(1.2)',   'true',  1;
    'MIL-SIL-03', 'single(0.5)',   'true',  2;
    'MIL-SIL-04', 'single(0.2)',   'true',  3;
    'MIL-SIL-05', 'single(-1)',    'true',  4;
    'MIL-SIL-06', 'single(NaN)',   'true',  4;
    'MIL-SIL-07', 'single(0.2)',   'false', 0;
    'MIL-SIL-08', 'single(1.5)',   'true',  1;
    'MIL-SIL-09', 'single(0.8)',   'true',  1;
    'MIL-SIL-10', 'single(0.3)',   'true',  2;
    'MIL-SIL-11', 'single(5)',     'true',  4;
    'MIL-SIL-12', 'single(Inf)',   'true',  4;
};

N = size(tests, 1);

testName = strings(N, 1);
expectedLevel = zeros(N, 1);
milLevel = nan(N, 1);
silLevel = nan(N, 1);
sameTime = false(N, 1);
sameWarning = false(N, 1);
sameBuzzer = false(N, 1);
passed = false(N, 1);

for k = 1:N

    testName(k) = string(tests{k,1});
    expectedLevel(k) = tests{k,4};

    warning = cell(1,2);
    buzzer = cell(1,2);

    for j = 1:2

        mdl = models{j};
        load_system(mdl);

        simIn = Simulink.SimulationInput(mdl);

        simIn = simIn.setBlockParameter( ...
            [mdl '/Distance_Test'], ...
            'Value', tests{k,2});

        simIn = simIn.setBlockParameter( ...
            [mdl '/ReverseGear_Test'], ...
            'Value', tests{k,3});

        simIn = simIn.setModelParameter( ...
            'StopTime', '2', ...
            'SolverType', 'Fixed-step', ...
            'Solver', 'FixedStepDiscrete', ...
            'FixedStep', '0.02');

        out = sim(simIn);

        warning{j} = out.get('WarningMIL');
        buzzer{j} = out.get('BuzzerMIL');

    end

    % Niveaux obtenus en fin de simulation
    milLevel(k) = double(warning{1}.Data(end));
    silLevel(k) = double(warning{2}.Data(end));

    % Comparaison des instants
    sameTime(k) = ...
        isequal(warning{1}.Time(:), warning{2}.Time(:)) && ...
        isequal(buzzer{1}.Time(:), buzzer{2}.Time(:));

    % Comparaison de tous les echantillons
    sameWarning(k) = isequal( ...
        warning{1}.Data(:), warning{2}.Data(:));

    sameBuzzer(k) = isequal( ...
        buzzer{1}.Data(:), buzzer{2}.Data(:));

    % Verifier le niveau attendu
    levelsOK = ...
        all(double(warning{1}.Data(:)) == expectedLevel(k)) && ...
        all(double(warning{2}.Data(:)) == expectedLevel(k));

    % Verifier le comportement general du buzzer
    buzzerMIL = double(buzzer{1}.Data(:));
    buzzerSIL = double(buzzer{2}.Data(:));

    switch expectedLevel(k)

        case {0,4}
            buzzerOK = all(buzzerMIL == 0) && ...
                       all(buzzerSIL == 0);

        case 3
            buzzerOK = all(buzzerMIL == 1) && ...
                       all(buzzerSIL == 1);

        otherwise
            buzzerOK = any(buzzerMIL == 0) && ...
                       any(buzzerMIL == 1) && ...
                       any(buzzerSIL == 0) && ...
                       any(buzzerSIL == 1);
    end

    passed(k) = sameTime(k) && sameWarning(k) && ...
                sameBuzzer(k) && levelsOK && buzzerOK;

    if passed(k)
        status = 'PASS';
    else
        status = 'FAIL';
    end

    fprintf('%s | MIL=%d | SIL=%d | %s\n', ...
        tests{k,1}, milLevel(k), silLevel(k), status);

end

% Tableau final
report = table(testName, expectedLevel, milLevel, ...
    silLevel, sameTime, sameWarning, sameBuzzer, passed);

disp('===== RESULTATS MIL / SIL =====');
disp(report);

fprintf('\nTests reussis : %d / %d\n', sum(passed), N);

% Exporter pour le rapport universitaire
writetable(report, 'PDW_MIL_SIL_results.csv');

if all(passed)
    disp('VALIDATION MIL/SIL : 12 TESTS REUSSIS');
else
    disp('VALIDATION MIL/SIL : ECHECS A ANALYSER');
end