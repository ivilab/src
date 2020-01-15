function plot_data_dir(data_dir, out_dir, index)
mkdir(out_dir);
    dirs = dir([data_dir '/*.txt']);
    for i = 1:size(dirs,1)
        data_file = [data_dir dirs(i).name]
        [pathstr, name, ext] = fileparts(dirs(i).name);
        out_fname = [out_dir name];
        plot_states(data_file, out_fname, index);
    end
end


