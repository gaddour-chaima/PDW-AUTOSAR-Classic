clc;
clear;

% Modele de test
mdl = 'PDW_TestBench';
load_system(mdl);

% Scenarios MIL
% Nom, Distance, ReverseGear, Niveau attendu

tests = {
    'MIL-01', 'single(2.0)',   'true',  0;
    'MIL-02', 'single(1.2)',   'true',  1;
    'MIL-03', 'single(0.5)',   'true',  2;
    'MIL-04', 'single(0.2)',   'true',  3;
    'MIL-05', 'single(-1)',    'true',  4;
    'MIL-06', 'single(NaN)',   'true',  4;
    'MIL-07', 'single(0.2)',   'false', 0;
    'MIL-08', 'single(1.5)',   'true',  1;
    'MIL-09', 'single(0.8)',   'true',  1;
    'MIL-10', 'single(0.3)',   'true',  2;
    'MIL-11', 'single(5)',     'true',  4;
    'MIL-12', 'single(Inf)',   'true',  4;
};

n = size(tests, 1);
results = false(n, 1);

fprintf('\n===== VALIDATION MIL PDW =====\n');

for i = 1:n

    % Configurer la simulation
    simIn = Simulink.SimulationInput(mdl);

    simIn = simIn.setBlockParameter( ...
        [mdl '/Distance_Test'], ...
        'Value', tests{i, 2});

    simIn = simIn.setBlockParameter( ...
        [mdl '/ReverseGear_Test'], ...
        'Value', tests{i, 3});

    simIn = simIn.setModelParameter( ...
        'StopTime', '2', ...
        'SolverType', 'Fixed-step', ...
        'Solver', 'FixedStepDiscrete', ...
        'FixedStep', '0.02');

    % Executer le modele
    out = sim(simIn);

    % Recuperer les sorties
    warning = out.get('WarningMIL');
    buzzer = out.get('BuzzerMIL');

    observed = double(warning.Data(end));
    expected = tests{i, 4};

    buzzerData = double(buzzer.Data(:));

    % Verifier le buzzer selon le niveau
    if expected == 0 || expected == 4
        buzzerOK = all(buzzerData == 0);
    elseif expected == 3
        buzzerOK = all(buzzerData == 1);
    else
        buzzerOK = any(buzzerData == 0) && ...
                   any(buzzerData == 1);
    end

    results(i) = (observed == expected) && buzzerOK;

    if results(i)
        status = 'PASS';
    else
        status = 'FAIL';
    end

    fprintf('%s : attendu=%d, obtenu=%d -> %s\n', ...
        tests{i,1}, expected, observed, status);

end

fprintf('\nTests reussis : %d / %d\n', sum(results), n);

if all(results)
    disp('VALIDATION MIL : TOUS LES TESTS ONT REUSSI');
else
    disp('VALIDATION MIL : ECHECS A ANALYSER');
end