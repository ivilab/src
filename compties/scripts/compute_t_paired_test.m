function compute_t_paired_test(base_fold_1, res_fold_2)

    %base_model_names = {'average', 'line' 'MAP-CLO-offset' 'MLE-CLO-offset'};
    base_model_names = {'average', 'line' 'MAP-CLO' 'MLE-CLO'};

    for i = 1:length(base_model_names)
        res_1 = [base_fold_1 '/models/' base_model_names{i} '/errors/err_couples.txt'];
        res_2 = [res_fold_2 '/errors/err_couples.txt'];

        fp1 = fopen(res_1, 'r');
        fp2 = fopen(res_2, 'r');
        before = load(res_1);
        after = load(res_2);

        fit_diff = after(:, 3) - before(:, 3);
        pred_diff = after(:, 5) - before(:, 5);

        [fit_t, fit_p] = get_t_statistics(fit_diff);
        [pred_diff, pred_p] = get_t_statistics(pred_diff);

        fprintf(1, '%20s fitting p_val: %5.2f; pred p_val: %5.2f\n', base_model_names{i}, fit_p, pred_p);
    end
end

function [t_score, p_value] = get_t_statistics(paired_diff)
    mean_diff = mean(paired_diff);
    sem_diff = std(paired_diff)/sqrt(length(paired_diff));
    t_score =  mean_diff/sem_diff;
    %tables of the t-distribution to compare your value for T
    p_value = (1-tcdf(abs(t_score),length(paired_diff)-1));
end
