
clc;

% 1. Executer la simulation validee
out = sim('PDW_System', 'StopTime', '4.8');

% 2. Recuperer les trois signaux
d = out.get('DistanceTrue3D');
w = out.get('Warning3D');
b = out.get('Buzzer3D');

% 3. Utiliser les instants de simulation
t = double(d.Time(:));
distance = double(d.Data(:));

% 4. Aligner les signaux discrets sur les memes instants
warning = interp1( ...
    double(w.Time(:)), double(w.Data(:)), ...
    t, 'previous', 'extrap');

buzzer = interp1( ...
    double(b.Time(:)), double(b.Data(:)), ...
    t, 'previous', 'extrap');

warning = round(warning);
buzzer = round(buzzer);

% 5. Construire le tableau
T = table(t, distance, warning, buzzer, ...
    'VariableNames', ...
    {'time_s','distance_m','warning_level','buzzer'});

% 6. Preparer le dossier Unity
folder = 'D:\Unity\Projects\PDW_Parking_3D\Assets\StreamingAssets';

if ~exist(folder, 'dir')
    mkdir(folder);
end

% 7. Exporter le fichier CSV
csvFile = fullfile(folder, 'pdw_simulation.csv');

writetable(T, csvFile, 'Delimiter', ',');

% 8. Verifier les resultats
fprintf('\n===== EXPORT PDW VERS UNITY =====\n');
fprintf('Fichier : %s\n', csvFile);
fprintf('Nombre echantillons : %d\n', height(T));
fprintf('Distance initiale : %.2f m\n', T.distance_m(1));
fprintf('Distance finale : %.2f m\n', T.distance_m(end));
fprintf('Warning final : %d\n', T.warning_level(end));
fprintf('Buzzer final : %d\n', T.buzzer(end));

disp('Export termine.');
