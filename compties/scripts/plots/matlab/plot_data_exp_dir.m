function plot_data_exp_dir(data_dir, exp_dir, out_dir, model_name)
mkdir(out_dir);
    dirs = dir([data_dir '/*.txt'])
    for i = 1:size(dirs,1)
        data_file = [data_dir '/' dirs(i).name]
        [pathstr, name, ext] = fileparts(dirs(i).name);
        out_fname = [out_dir '/' name];
        exp_fname = [exp_dir '/' name '/all_states/states.txt'];
        plot_data_exp(data_file, exp_fname, out_fname, model_name);
    end
end

